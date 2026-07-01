/* Copyright (C) 2001 Vasili Gavrilov. GNU GPL v2 or later. Hardened 2026. */
/* main.c -- the CLI front end: parse options, get one input (an argument, or a
 * line from stdin so it works as a filter), run process(), print the result.
 * The scaffolding stays; a new project changes process.c, not this file. */
#include "common.h"
#include "process.h"
#include "constants.h"

#include <stdio.h>
#include <string.h>
#include <getopt.h>

#ifndef UNIT_TEST   /* the test build (make ut) supplies main() from tests.c */

static void usage(FILE *out, const char *prog) {
    fprintf(out,
        "usage: %s [-d] [-h] [INPUT]\n"
        "  with INPUT: process that argument\n"
        "  without:    read one line from stdin (filter mode)\n"
        "  -d  debug tracing to stderr\n"
        "  -h  this help\n", prog);
}

int main(int argc, char **argv) {
    int c;
    const char *input;
    char line[MAX_INPUT];
    char out[MAX_OUTPUT];

    while ((c = getopt(argc, argv, "dh")) != -1) {
        switch (c) {
            case 'd': g_debug = 1; break;
            case 'h': usage(stdout, argv[0]); return 0;
            default:  usage(stderr, argv[0]); return 2;
        }
    }

    if (optind < argc) {
        input = argv[optind];
    } else {
        if (!fgets(line, sizeof line, stdin)) { usage(stderr, argv[0]); return 2; }
        line[strcspn(line, "\r\n")] = '\0';
        input = line;
    }

    debug("processing '%s'", input);
    if (process(input, out, sizeof out) != 0)
        die("process failed (output buffer too small?)");

    printf("%s\n", out);
    return 0;
}

#endif /* !UNIT_TEST */
