# Copyright (C) 2001 Vasili Gavrilov. GNU GPL v2 or later. Hardened 2026.
#
# ais-style build. Honors the standard variables (CC CFLAGS CPPFLAGS LDFLAGS
# LDLIBS); project-required flags are APPENDED, never override yours. Drop in a
# .c -- here or one directory down -- and it compiles, no editing this file.
#   make | make release | make debug | make pedantic | make ut | make clean
SHELL = /bin/sh

BIN     = aisconfig
TESTBIN = aisconfig_ut

CC       ?= cc
CFLAGS   ?= -O2
CPPFLAGS ?=
LDFLAGS  ?=
LDLIBS   ?=

# project-required flags, applied alongside (not over) the user's
STD  = -std=c99
WARN = -W -Wall
PROJ = $(STD) $(WARN)

# every *.c at top level or one dir down; add a file, no edit needed
SOURCES.c := $(wildcard *.c) $(wildcard */*.c)
OBJS       = $(SOURCES.c:.c=.o)

debug    : CFLAGS = -g -O0
debug    : WARN  += -Wundef
pedantic : STD    = -std=c99 -pedantic
pedantic : WARN  += -Wundef -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations
release  : CFLAGS = -O2

.SUFFIXES:
.SUFFIXES: .d .o .h .c
%.o: %.c
	$(CC) $(PROJ) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

.PHONY: all release debug pedantic ut clean
all release debug pedantic: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(PROJ) $(CFLAGS) $(LDFLAGS) -o $(BIN) $(OBJS) $(LDLIBS)

# ut: all sources with -DUNIT_TEST -- main.c's main() compiles out, tests.c's in.
$(TESTBIN): $(SOURCES.c)
	$(CC) $(PROJ) -g -DUNIT_TEST $(CPPFLAGS) $(SOURCES.c) -o $(TESTBIN) $(LDLIBS)
ut: $(TESTBIN)
	./$(TESTBIN)

clean:
	-rm -f $(BIN) $(TESTBIN) $(OBJS) $(OBJS:.o=.d)

-include $(OBJS:.o=.d)
