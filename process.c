/* Copyright (C) 2026 Vasili Gavrilov. GNU GPL v2 or later. */
/* process.c -- the example processor: the Base64 of the input string. It is
 * one public, dependency-free processor to demonstrate the slot (and to give
 * base64.c a caller); replace the body with your own logic and keep the
 * signature. */
#include "process.h"
#include "base64.h"

#include <string.h>

int process(const char *input, char *out, size_t outsz) {
    size_t inlen  = strlen(input);
    size_t enclen = ((inlen + 2) / 3) * 4;         /* Base64 expands 3 -> 4 */

    if (enclen + 1 > outsz) return -1;             /* need room for chars + NUL */

    if (base64_encode((unsigned)inlen, input, (unsigned)outsz, out) != 0)
        return -1;
    out[enclen] = '\0';
    return 0;
}
