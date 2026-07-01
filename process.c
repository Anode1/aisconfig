/* Copyright (C) 2026 Vasili Gavrilov. GNU GPL v2 or later. */
/* process.c -- the example processor: a short, URL-safe-ish id for a string,
 * being the Base64 of its MD5 digest with the trailing '=' padding dropped
 * (16 bytes -> 24 Base64 chars, last two are padding, so 22 significant chars).
 * This is the one public, dependency-free processor to demonstrate the slot;
 * replace the body with your own logic and keep the signature. */
#include "process.h"
#include "md5.h"
#include "base64.h"

#include <string.h>

int process(const char *input, char *out, size_t outsz) {
    MD5_CTX ctx;
    char b64[25];                                  /* 24 chars + NUL */

    MD5Init(&ctx);
    MD5Update(&ctx, (const unsigned char *)input, (unsigned int)strlen(input));
    MD5Final(&ctx);

    if (base64_encode(16, (const char *)ctx.digest, sizeof b64, b64) != 0)
        return -1;
    b64[22] = '\0';                                /* drop the "==" padding */

    if (strlen(b64) + 1 > outsz) return -1;
    memcpy(out, b64, strlen(b64) + 1);
    return 0;
}