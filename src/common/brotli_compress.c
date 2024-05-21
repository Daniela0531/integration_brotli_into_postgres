#ifndef FRONTEND
#include "postgres.h"
#else
#include "postgres_fe.h"
#endif

#include <limits.h>

#include "common/brotli_compress.h"
#include "common/encode.h"


/* ----------
 * Local definitions
 * ----------
 */
#define BROTLI_MAX_HISTORY_LISTS	8192	/* must be power of 2 */
#define BROTLI_HISTORY_SIZE		    4096
#define BROTLI_MAX_MATCH			273


/*
 * Element 0 in hist_entries is unused, and means 'invalid'. Likewise,
 * INVALID_ENTRY_PTR in next/prev pointers mean 'invalid'.
 */
#define INVALID_ENTRY			0
#define INVALID_ENTRY_PTR		(&hist_entries[INVALID_ENTRY])


// now I have:
// BROTLI_BOOL BrotliEncoderCompress(
//        int quality, int lgwin, BrotliEncoderMode mode, size_t input_size,
//        const uint8_t input_buffer[BROTLI_ARRAY_PARAM(input_size)],
//        size_t* encoded_size,
//        uint8_t encoded_buffer[BROTLI_ARRAY_PARAM(*encoded_size)])

////// TODO ///////

int32
brotli_compress(const char *source, int32 slen, char *dest)
{
    size_t input_size = (size_t)slen;
    const uint8_t input_buffer[BROTLI_ARRAY_PARAM(input_size)] = (const uint8_t *)source;
    size_t* encoded_size = len(encoded_buffer);
    uint8_t encoded_buffer[BROTLI_ARRAY_PARAM(*encoded_size)] = (uint8_t*)encoded_buffer;
    BROTLI_BOOL is_sucess = BrotliEncoderCompress(
            BROTLI_DEFAULT_QUALITY,
            BROTLI_DEFAULT_WINDOW,
            BROTLI_DEFAULT_MODE,
            input_size,
            input_buffer,
            encoded_size,
            encoded_buffer);

    if (!is_sucess) {
        return -1;
    }
    return (int32)*encoded_size;
}


/* ----------
 * pglz_decompress -
 *
 *		Decompresses source into dest. Returns the number of bytes
 *		decompressed into the destination buffer, or -1 if the
 *		compressed data is corrupted.
 *
 *		If check_complete is true, the data is considered corrupted
 *		if we don't exactly fill the destination buffer.  Callers that
 *		are extracting a slice typically can't apply this check.
 * ----------
 */
int32
brotli_decompress(const char *source, int32 slen, char *dest,
                int32 rawsize, bool check_complete)
{
    const unsigned char *sp;
    const unsigned char *srcend;
    unsigned char *dp;
    unsigned char *destend;

    sp = (const unsigned char *) source;
    srcend = ((const unsigned char *) source) + slen;
    dp = (unsigned char *) dest;
    destend = dp + rawsize;

    while (sp < srcend && dp < destend)
    {
        /*
         * Read one control byte and process the next 8 items (or as many as
         * remain in the compressed input).
         */
        unsigned char ctrl = *sp++;
        int			ctrlc;

        for (ctrlc = 0; ctrlc < 8 && sp < srcend && dp < destend; ctrlc++)
        {
            if (ctrl & 1)
            {
                /*
                 * Set control bit means we must read a match tag. The match
                 * is coded with two bytes. First byte uses lower nibble to
                 * code length - 3. Higher nibble contains upper 4 bits of the
                 * offset. The next following byte contains the lower 8 bits
                 * of the offset. If the length is coded as 18, another
                 * extension tag byte tells how much longer the match really
                 * was (0-255).
                 */
                int32		len;
                int32		off;

                len = (sp[0] & 0x0f) + 3;
                off = ((sp[0] & 0xf0) << 4) | sp[1];
                sp += 2;
                if (len == 18)
                    len += *sp++;

                /*
                 * Check for corrupt data: if we fell off the end of the
                 * source, or if we obtained off = 0, or if off is more than
                 * the distance back to the buffer start, we have problems.
                 * (We must check for off = 0, else we risk an infinite loop
                 * below in the face of corrupt data.  Likewise, the upper
                 * limit on off prevents accessing outside the buffer
                 * boundaries.)
                 */
                if (unlikely(sp > srcend || off == 0 ||
                             off > (dp - (unsigned char *) dest)))
                    return -1;

                /*
                 * Don't emit more data than requested.
                 */
                len = Min(len, destend - dp);

                /*
                 * Now we copy the bytes specified by the tag from OUTPUT to
                 * OUTPUT (copy len bytes from dp - off to dp).  The copied
                 * areas could overlap, so to avoid undefined behavior in
                 * memcpy(), be careful to copy only non-overlapping regions.
                 *
                 * Note that we cannot use memmove() instead, since while its
                 * behavior is well-defined, it's also not what we want.
                 */
                while (off < len)
                {
                    /*
                     * We can safely copy "off" bytes since that clearly
                     * results in non-overlapping source and destination.
                     */
                    memcpy(dp, dp - off, off);
                    len -= off;
                    dp += off;

                    /*----------
                     * This bit is less obvious: we can double "off" after
                     * each such step.  Consider this raw input:
                     *		112341234123412341234
                     * This will be encoded as 5 literal bytes "11234" and
                     * then a match tag with length 16 and offset 4.  After
                     * memcpy'ing the first 4 bytes, we will have emitted
                     *		112341234
                     * so we can double "off" to 8, then after the next step
                     * we have emitted
                     *		11234123412341234
                     * Then we can double "off" again, after which it is more
                     * than the remaining "len" so we fall out of this loop
                     * and finish with a non-overlapping copy of the
                     * remainder.  In general, a match tag with off < len
                     * implies that the decoded data has a repeat length of
                     * "off".  We can handle 1, 2, 4, etc repetitions of the
                     * repeated string per memcpy until we get to a situation
                     * where the final copy step is non-overlapping.
                     *
                     * (Another way to understand this is that we are keeping
                     * the copy source point dp - off the same throughout.)
                     *----------
                     */
                    off += off;
                }
                memcpy(dp, dp - off, len);
                dp += len;
            }
            else
            {
                /*
                 * An unset control bit means LITERAL BYTE. So we just copy
                 * one from INPUT to OUTPUT.
                 */
                *dp++ = *sp++;
            }

            /*
             * Advance the control bit
             */
            ctrl >>= 1;
        }
    }

    /*
     * If requested, check we decompressed the right amount.
     */
    if (check_complete && (dp != destend || sp != srcend))
        return -1;

    /*
     * That's it.
     */
    return (char *) dp - dest;
}


/* ----------
 * pglz_maximum_compressed_size -
 *
 *		Calculate the maximum compressed size for a given amount of raw data.
 *		Return the maximum size, or total compressed size if maximum size is
 *		larger than total compressed size.
 *
 * We can't use PGLZ_MAX_OUTPUT for this purpose, because that's used to size
 * the compression buffer (and abort the compression). It does not really say
 * what's the maximum compressed size for an input of a given length, and it
 * may happen that while the whole value is compressible (and thus fits into
 * PGLZ_MAX_OUTPUT nicely), the prefix is not compressible at all.
 * ----------
 */
int32
brotli_maximum_compressed_size(int32 rawsize, int32 total_compressed_size)
{
    int64		compressed_size;

    /*
     * pglz uses one control bit per byte, so if the entire desired prefix is
     * represented as literal bytes, we'll need (rawsize * 9) bits.  We care
     * about bytes though, so be sure to round up not down.
     *
     * Use int64 here to prevent overflow during calculation.
     */
    compressed_size = ((int64) rawsize * 9 + 7) / 8;

    /*
     * The above fails to account for a corner case: we could have compressed
     * data that starts with N-1 or N-2 literal bytes and then has a match tag
     * of 2 or 3 bytes.  It's therefore possible that we need to fetch 1 or 2
     * more bytes in order to have the whole match tag.  (Match tags earlier
     * in the compressed data don't cause a problem, since they should
     * represent more decompressed bytes than they occupy themselves.)
     */
    compressed_size += 2;

    /*
     * Maximum compressed size can't be larger than total compressed size.
     * (This also ensures that our result fits in int32.)
     */
    compressed_size = Min(compressed_size, total_compressed_size);

    return (int32) compressed_size;
}
