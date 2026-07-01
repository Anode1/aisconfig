/* Copyright (C) 2026 Vasili Gavrilov. GNU GPL v2 or later. */
/* tests.c -- in-place unit tests, run by `make ut` (which builds every source
 * with -DUNIT_TEST; this file is empty otherwise, and main.c's main() is then
 * compiled out). Add a CHECK when you add a feature. Idempotent. */
#ifdef UNIT_TEST

#include "common.h"
#include "utils.h"
#include "hash.h"
#include "params.h"
#include "md5.h"
#include "base64.h"
#include "process.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int pass, fail;
#define CHECK(cond, msg) do { \
    if (cond) pass++; \
    else { fail++; printf("  FAIL %s  (%s:%d)\n", msg, __FILE__, __LINE__); } \
} while (0)

static void md5_hex(const char *s, char out[33]) {
    MD5_CTX c; int i;
    MD5Init(&c);
    MD5Update(&c, (const unsigned char *)s, (unsigned)strlen(s));
    MD5Final(&c);
    for (i = 0; i < 16; i++) snprintf(out + 2 * i, 3, "%02x", c.digest[i]);
    out[32] = '\0';
}

static void test_utils(void) {
    char a[16];
    strcpy(a, "  hi  ");
    rtrim(a, ' '); CHECK(strcmp(a, "  hi") == 0, "rtrim trailing");
    ltrim(a, ' '); CHECK(strcmp(a, "hi") == 0, "ltrim leading");
    strcpy(a, "xxab"); ltrim(a, 'x'); CHECK(strcmp(a, "ab") == 0, "ltrim run");
}

static void test_hash(void) {
    struct hash *h = hash_create(16);
    hash_put(h, "k", "one");
    CHECK(strcmp((char *)hash_get(h, "k"), "one") == 0, "hash get");
    hash_put(h, "k", "two");
    CHECK(strcmp((char *)hash_get(h, "k"), "two") == 0, "hash replace");
    CHECK(hash_get(h, "absent") == NULL, "hash miss");
    CHECK(strcmp((char *)hash_delete_entry(h, "k"), "two") == 0, "hash delete");
    CHECK(hash_get(h, "k") == NULL, "hash gone after delete");
    hash_delete(h);
}

static void test_params(void) {
    CHECK(params_load("system.properties") == 0, "params load");
    CHECK(params_get("greeting") && strcmp(params_get("greeting"), "hello") == 0, "params value");
    CHECK(params_get("result.length") && strcmp(params_get("result.length"), "22") == 0, "params dotted key");
    CHECK(params_get("nope") == NULL, "params miss");
    CHECK(params_load("no-such-file") == -1, "params open error");
    params_free();
}

static void test_md5(void) {
    char h[33];
    md5_hex("", h);    CHECK(strcmp(h, "d41d8cd98f00b204e9800998ecf8427e") == 0, "md5 of empty");
    md5_hex("abc", h); CHECK(strcmp(h, "900150983cd24fb0d6963f7d28e17f72") == 0, "md5 of abc");
}

static void test_base64(void) {
    char enc[16], dec[16];
    CHECK(base64_encode(5, "hello", sizeof enc, enc) == 0, "b64 encode ok");
    enc[8] = '\0';
    CHECK(strcmp(enc, "aGVsbG8=") == 0, "b64 encode value");
    CHECK(base64_decode(8, enc, sizeof dec, dec) == 0, "b64 decode ok");
    CHECK(memcmp(dec, "hello", 5) == 0, "b64 roundtrip");
}

static void test_process(void) {
    char a[MAX_OUTPUT], b[MAX_OUTPUT];
    CHECK(process("abc", a, sizeof a) == 0, "process ok");
    CHECK(strlen(a) == 22, "process id length");
    CHECK(strcmp(a, "kAFQmDzST7DWlj99KOF/cg") == 0, "process id value");
    process("abc", b, sizeof b); CHECK(strcmp(a, b) == 0, "process deterministic");
    process("abd", b, sizeof b); CHECK(strcmp(a, b) != 0, "process distinguishes");
    CHECK(process("abc", a, 4) == -1, "process guards small buffer");
}

int main(void) {
    test_utils();
    test_hash();
    test_params();
    test_md5();
    test_base64();
    test_process();
    printf("ut: %d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}

#else
typedef int tests_translation_unit_not_empty;  /* ISO C forbids an empty TU */
#endif /* UNIT_TEST */
