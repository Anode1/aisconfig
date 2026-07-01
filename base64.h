/* base64.h -- Base64 encode/decode over raw byte buffers, bounded by dlen.
 * encode/decode functions are by Lew Pitcher. */
#ifndef BASE64_H
#define BASE64_H

/* Encode slen bytes of src into dst (capacity dlen). Returns 0 on success,
 * non-zero if dst is too small. */
int base64_encode(unsigned slen, const char *src, unsigned dlen, char *dst);

/* Decode slen base64 chars of src into dst (capacity dlen). Returns 0 on
 * success, non-zero on underflow/overflow. */
int base64_decode(unsigned slen, const char *src, unsigned dlen, char *dst);

#endif /* BASE64_H */
