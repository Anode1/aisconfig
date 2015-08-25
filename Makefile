#
# Copyright (C) 2001 Vasili Gavrilov
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

SHELL=/bin/sh

BIN=test

ARCH := `uname`

NOCYGWIN=

ifeq ($(ARCH), CYGWIN32/95)
	NOCYGWIN= -mno-cygwin
endif
ifeq ($(ARCH), CYGWIN32/NT)
	NOCYGWIN= -mno-cygwin
endif


#consider all *.c as sources  
SOURCES.c := $(wildcard *.c)


CFLAGS= $(NOCYGWIN) -ansi -W -Wall
CPPFLAGS=
CC=gcc
SLIBS=
INCLUDES=
OBJS=$(SOURCES.c:.c=.o)
LINK=gcc $(CFLAGS)
LFLAGS=-lm $()

debug : CFLAGS = $(NOCYGWIN) -ansi -W -Wall -g -Wundef
pedantic : CFLAGS = $(NOCYGWIN) -ansi -W -Wall -g -Wundef -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations
release : CFLAGS = $(NOCYGWIN) -ansi -W -Wall -O2
ut : CFLAGS = $(NOCYGWIN) -ansi -g -DUNIT_TEST -DHASH_UNIT_TEST

.SUFFIXES:
.SUFFIXES: .d .o .h .c
.c.o: ; $(CC) $(INCLUDES) $(CFLAGS) -MMD -c $*.c 

.PHONY: clean

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

all release debug pedantic ut: $(BIN)

$(BIN): $(OBJS)
	$(LINK) $(FLAGS) -o $(BIN) $(OBJS) $(LFLAGS)

clean:
	-rm -f $(BIN) $(OBJS) *.d


include $(sources:.c=.d)

