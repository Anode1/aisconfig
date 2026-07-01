/*
 **********************************************************************
 ** md5.h -- Header file for implementation of MD5                   **
 ** RSA Data Security, Inc. MD5 Message Digest Algorithm             **
 ** Created: 2/17/90 RLR                                             **
 ** Revised (for MD5): RLR 4/27/91                                   **
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function. These notices must be retained in any **
 ** copies of any part of this documentation and/or software.        **
 **********************************************************************
 *
 * Modernized: UINT4 is uint32_t (the algorithm needs exactly 32 bits; a plain
 * `unsigned long` is 64-bit on LP64 and silently produces wrong digests), and
 * the K&R prototypes are now ANSI.
 */
#ifndef MD5_H
#define MD5_H

#include <stdint.h>

typedef uint32_t UINT4;            /* exactly 32 bits */

typedef struct {
    UINT4         i[2];            /* number of _bits_ handled mod 2^64 */
    UINT4         buf[4];          /* scratch buffer                    */
    unsigned char in[64];          /* input buffer                      */
    unsigned char digest[16];      /* the digest after MD5Final         */
} MD5_CTX;

void MD5Init(MD5_CTX *ctx);
void MD5Update(MD5_CTX *ctx, const unsigned char *inBuf, unsigned int inLen);
void MD5Final(MD5_CTX *ctx);

#endif /* MD5_H */
