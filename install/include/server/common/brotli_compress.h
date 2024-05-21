//
// Created by Daniela on 20.05.2024.
//

//// TODO
/// my_realisation ///////

#ifndef BROTLI_INTO_POSTGRES_BROTLI_COMPRESSION_H
#define BROTLI_INTO_POSTGRES_BROTLI_COMPRESSION_H

#endif //BROTLI_INTO_POSTGRES_BROTLI_COMPRESSION_H
//
///* ----------
// * pg_lzcompress.h -
// *
// *	Definitions for the builtin LZ compressor
// *
// * src/include/common/pg_lzcompress.h
// * ----------
// */
//
//#ifndef _PG_LZCOMPRESS_H_
//#define _PG_LZCOMPRESS_H_
//
//
///* ----------
// * PGLZ_MAX_OUTPUT -
// *
// *		Macro to compute the buffer size required by pglz_compress().
// *		We allow 4 bytes for overrun before detecting compression failure.
// * ----------
// */
//#define PGLZ_MAX_OUTPUT(_dlen)			((_dlen) + 4)
//
//
///* ----------
// * PGLZ_Strategy -
// *
// *		Some values that control the compression algorithm.
// *
// *		min_input_size		Minimum input data size to consider compression.
// *
// *		max_input_size		Maximum input data size to consider compression.
// *
// *		min_comp_rate		Minimum compression rate (0-99%) to require.
// *							Regardless of min_comp_rate, the output must be
// *							smaller than the input, else we don't store
// *							compressed.
// *
// *		first_success_by	Abandon compression if we find no compressible
// *							data within the first this-many bytes.
// *
// *		match_size_good		The initial GOOD match size when starting history
// *							lookup. When looking up the history to find a
// *							match that could be expressed as a tag, the
// *							algorithm does not always walk back entirely.
// *							A good match fast is usually better than the
// *							best possible one very late. For each iteration
// *							in the lookup, this value is lowered so the
// *							longer the lookup takes, the smaller matches
// *							are considered good.
// *
// *		match_size_drop		The percentage by which match_size_good is lowered
// *							after each history check. Allowed values are
// *							0 (no change until end) to 100 (only check
// *							latest history entry at all).
// * ----------
// */
//typedef struct PGLZ_Strategy
//{
//    int32		min_input_size;
//    int32		max_input_size;
//    int32		min_comp_rate;
//    int32		first_success_by;
//    int32		match_size_good;
//    int32		match_size_drop;
//} PGLZ_Strategy;
//
//
///* ----------
// * The standard strategies
// *
// *		PGLZ_strategy_default		Recommended default strategy for TOAST.
// *
// *		PGLZ_strategy_always		Try to compress inputs of any length.
// *									Fallback to uncompressed storage only if
// *									output would be larger than input.
// * ----------
// */
//extern PGDLLIMPORT const PGLZ_Strategy *const PGLZ_strategy_default;
//extern PGDLLIMPORT const PGLZ_Strategy *const PGLZ_strategy_always;
//

/* ----------
 * Global function declarations
 * ----------
 */

// Returns the number of bytes written in buffer dest, or -1 if compression fails.
// dest: Compresses source into dest using strategy.
// strategy: PGLZ_Strategy
extern int32 brotli_compress(const char *source, int32 slen, char *dest,
                           const PGLZ_Strategy *strategy);

// now I have:
// BROTLI_BOOL BrotliEncoderCompress(
//        int quality, int lgwin, BrotliEncoderMode mode, size_t input_size,
//        const uint8_t input_buffer[BROTLI_ARRAY_PARAM(input_size)],
//        size_t* encoded_size,
//        uint8_t encoded_buffer[BROTLI_ARRAY_PARAM(*encoded_size)])

// but I need
// int32 BrotliEncoderCompress(
//        int32 input_size,
//        char *input_buffer,
//        char *encoded_buffer)

// Returns the number of bytes decompressed into the destination buffer
// or -1 if the compressed data is corrupted.
//
// dest:           Decompresses source into dest.

// check_complete: Если check_complete имеет значение true,
//                 данные считаются поврежденными, если мы
//                 не полностью заполняем целевой буфер.
//                 Вызывающие программы, извлекающие фрагмент,
//                 обычно не могут применить эту проверку.
extern int32 brotli_decompress(const char *source, int32 slen, char *dest,
                             int32 rawsize, bool check_complete);
// now I have:
// BROTLI_DEC_API BrotliDecoderResult BrotliDecoderDecompress(
//        size_t encoded_size,
//        const uint8_t encoded_buffer[BROTLI_ARRAY_PARAM(encoded_size)],
//        size_t* decoded_size,
//        uint8_t decoded_buffer[BROTLI_ARRAY_PARAM(*decoded_size)]);

// But I need:
// int32 BrotliDecoderDecompress(
//        char *encoded_buffer,
//        int32 encoded_size,
//        int32 decoded_size,
//        char *decoded_buffer, // ????
//        bool check_complete);
extern int32 brotli_maximum_compressed_size(int32 rawsize,
                                          int32 total_compressed_size);
// now I have:
// BROTLI_ENC_API size_t BrotliEncoderMaxCompressedSize(size_t input_size);

// But I need:
// int32 BrotliEncoderMaxCompressedSize(int32 input_size, //????//);

#endif							/* _PG_LZCOMPRESS_H_ */
