/* Copyright (C) 2026 Vasili Gavrilov. GNU GPL v2 or later. */
/* process.h -- THE pluggable slot. A new project replaces process() with its
 * own filter/processor logic; main.c and the tests stay as they are. */
#ifndef PROCESS_H
#define PROCESS_H

#include <stddef.h>

/* Transform one input string into a NUL-terminated result in out[outsz].
 * Returns 0 on success, -1 on error (e.g. out too small). */
int process(const char *input, char *out, size_t outsz);

#endif /* PROCESS_H */