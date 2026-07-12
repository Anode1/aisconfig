# aisconfig -- a C project template

A ready-to-extend C99 skeleton with the scaffolding already correct: build,
config, in-place tests, safe primitives, and a CLI that reads an argument or
stdin. Clone it, drop your logic into `process()`, add a property, add a test,
run `make ut`. The point is to start a new C project without re-deciding how to
build, how to test, or how to write it safely.

## Build and run

    make            # build ./aisconfig
    make ut         # build and run the in-place unit tests
    make ut-asan    # run the tests under AddressSanitizer + UBSan
    make debug      # -g -DDEBUG
    make pedantic   # strict warnings (-pedantic -Wshadow -Wstrict-prototypes ...)
    make release    # -O2 -DNDEBUG
    make clean

    ./aisconfig abc              # process an argument
    echo abc | ./aisconfig       # or read one line from stdin (filter mode)
    ./aisconfig -d abc           # with debug tracing on stderr

The default `process()` is one public, dependency-free example: the Base64 of the
input string. `base64.c` is a leftover from a real production project, kept only
as an example utility the default processor uses; delete it when you replace
`process()` with your own.

## Layout

```
main.c            CLI front end: options, one input (arg or stdin), print result
process.c/.h      THE slot -- your filter/processor logic goes here
common.c/.h       safe primitives: die(), debug(), xmalloc(), xstrdup()
utils.c/.h        bounded string helpers (rtrim/ltrim)
params.c/.h       config: load system.properties, params_get("key")
hash.c/.h         generic string -> void* hash table (backs params)
system.properties example config
base64.c/.h       example utility used by the default processor
constants.h       tunable sizes
tests.c           in-place unit tests (make ut)
Makefile          the build
```

## Extend

- **New processor / filter:** edit `process.c`, keep the `process()` signature.
  `main.c` and the tests need no change.
- **New config knob:** add `key = value` to `system.properties`, read it with
  `params_get("key")`.
- **New test:** add a `CHECK(condition, "name")` in `tests.c`; run `make ut`.
- **Rename:** set `BIN` in the `Makefile`.

## Style

The rules the code already follows, so new code matches:

- **C99, warning-free.** Build clean under `-std=c99 -Wall -Wextra`; a warning is
  a defect. `make pedantic` is the stricter gate.
- **Bounded strings only.** `snprintf` always; never `strcpy`/`strcat`/`sprintf`/
  `vsprintf`. Buffer sizes come from `constants.h`.
- **Checked allocation.** `xmalloc`/`xstrdup` never return NULL (they `die` on
  out-of-memory); free on every path.
- **Functions, not fragile macros.** `die`, `debug`, `xmalloc` are functions, so
  they type-check and are greppable. No `FATAL(x)` that passes user text as a
  format string.
- **Modules return, the CLI exits.** A module returns `0`/`-1` and lets the
  caller decide; only `main` (via `die`) terminates.
- **One concept per file**, `static` for anything module-private, `const`-correct,
  `size_t` for sizes.
- **Tests in place.** `make ut` is already wired; a feature ships with a `CHECK`.
- **Sanitizer-clean.** `make ut-asan` runs the tests under AddressSanitizer and
  UBSan; a leak, overflow, or undefined behavior is a defect. Run it before
  tagging.

These are the working subset. The full rationale, stack-first and bounded-memory
to the avionics and medical-device standard (NASA Power of Ten, MISRA C:2012
rule 21.3), is written up in `ais` (see below): this skeleton is one instance of
it, not the argument for it.

## See also

- [ais](https://github.com/Anode1/ais) - the associative-memory engine these
  conventions come from; its
  [`doc/dev/STYLE.md`](https://github.com/Anode1/ais/blob/main/doc/dev/STYLE.md)
  is the full coding-style rationale (the MISRA / Power-of-Ten discipline the
  Style section above condenses).
- [agent-recipes](https://github.com/Anode1/agent-recipes) - short prompts for
  working with coding agents; this template is the C starter it points to.

## Third-party notices

`base64.c` encode/decode are by Lew Pitcher. Everything else is under `LICENSE`.
