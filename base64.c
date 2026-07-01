/* base64.c -- see base64.h. encode/decode functions are by Lew Pitcher. */
#include "base64.h"

static const char base64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_encode(unsigned slen, const char *src, unsigned dlen, char *dst) {
    unsigned triad;
    for (triad = 0; triad < slen; triad += 3) {
        unsigned long sr = 0;
        unsigned byte;
        for (byte = 0; byte < 3 && triad + byte < slen; ++byte) {
            sr <<= 8;
            sr |= (unsigned char)src[triad + byte];
        }
        sr <<= (6 - ((8 * byte) % 6)) % 6;        /* left-align to 6-bit groups */

        if (dlen < 4) return 1;                    /* dest too short */
        dst[0] = dst[1] = dst[2] = dst[3] = '=';
        switch (byte) {
            case 3: dst[3] = base64[sr & 0x3f]; sr >>= 6;  /* fall through */
            case 2: dst[2] = base64[sr & 0x3f]; sr >>= 6;  /* fall through */
            case 1: dst[1] = base64[sr & 0x3f]; sr >>= 6;
                    dst[0] = base64[sr & 0x3f];
        }
        dst += 4; dlen -= 4;
    }
    return 0;
}

/* which sextet value a base64 character represents, or -1 */
static int tlu(int ch) {
    int i;
    for (i = 0; i < 64; ++i)
        if (base64[i] == ch) return i;
    return -1;
}

int base64_decode(unsigned slen, const char *src, unsigned dlen, char *dst) {
    unsigned six, dix = 0;
    for (six = 0; six < slen; six += 4) {
        unsigned long sr = 0;
        unsigned ix;
        for (ix = 0; ix < 4; ++ix) {
            int sextet;
            if (six + ix >= slen) return 1;        /* underflow */
            if ((sextet = tlu(src[six + ix])) < 0) break;
            sr <<= 6;
            sr |= (unsigned long)(sextet & 0x3f);
        }
        switch (ix) {
            case 0: return 0;                       /* end, no padding */
            case 1: return 2;                       /* can't happen    */
            case 2:
                sr >>= 4;
                if (dix >= dlen) return 3;
                dst[dix++] = (char)(sr & 0xff);
                break;
            case 3:
                sr >>= 2;
                if (dix + 1 >= dlen) return 3;
                dst[dix + 1] = (char)(sr & 0xff); sr >>= 8;
                dst[dix]     = (char)(sr & 0xff);
                dix += 2;
                break;
            case 4:
                if (dix + 2 >= dlen) return 3;
                dst[dix + 2] = (char)(sr & 0xff); sr >>= 8;
                dst[dix + 1] = (char)(sr & 0xff); sr >>= 8;
                dst[dix]     = (char)(sr & 0xff);
                dix += 3;
                break;
        }
    }
    return 0;
}
