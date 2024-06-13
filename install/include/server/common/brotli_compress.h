//
// Created by Daniela on 20.05.2024.
//

//// TODO
/// my realization ///////

// #ifndef BROTLI_INTO_POSTGRES_BROTLI_COMPRESSION_H
// #define BROTLI_INTO_POSTGRES_BROTLI_COMPRESSION_H

// #endif BROTLI_INTO_POSTGRES_BROTLI_COMPRESSION_H

#ifndef _BROTLI_COMPRESS_H_
#define _BROTLI_COMPRESS_H_

/* ----------
 * Global function declarations
 * ----------
 */

// Returns the number of bytes written in buffer dest, or -1 if compression fails.
// dest: Compresses source into dest using strategy.
extern int32 brotli_compress(const char *source, int32 slen, char *dest);


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

#endif							/* _BROTLI_COMPRESS_H_ */
