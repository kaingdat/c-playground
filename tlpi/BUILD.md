# Build Guide

GNU Make build system producing a static library (`libtlpi.a`) and example programs.

## Quick Start

```bash
make all                # build everything (lib, then each program directory)
make allgen              # build only the portable (GEN_EXE) programs
make clean                # remove generated files everywhere
cd fileio && make copy  # build a single program
cd fileio && make showall # list the executables defined in a directory
```

## Architecture

```
Makefile (root)
├── DIRS = lib fileio        # dirs are built in this order, sequentially
│
├── Makefile.inc             # shared: CFLAGS, TLPI_LIB, TLPI_INCL_DIR
│   ├── IMPL_CFLAGS          # -std=c99 -Wall -W ... pedantic warnings
│   ├── TLPI_LIB = ../libtlpi.a
│   ├── LINUX_LIB{RT,DL,ACL,CRYPT,CAP}, IMPL_THREAD_FLAGS
│   │   # -lrt/-ldl/-lacl/-lcrypt/-lcap/-pthread building blocks;
│   │   # unused until a directory's own Makefile opts in via LDLIBS
│   └── Extra clang warning suppressions, gated on `CC=clang`
│
├── lib/
│   ├── Makefile             # wildcard *.c → compile → ar rs libtlpi.a
│   ├── Build_ename.sh       # parses errno headers → ename.c.inc
│   └── error_functions.o depends on ename.c.inc (generated)
│
└── fileio/
    └── Makefile             # GEN_EXE + LINUX_EXE lists → link against libtlpi.a
```

**Build flow**: The root Makefile loops over `DIRS` in the order listed (`lib`
first, then `fileio`) and runs `make` in each, stopping at the first
directory that fails. This ordering is just the sequence of the shell loop,
not a make-level dependency graph — `lib` must stay first in `DIRS` for the
library to exist before other directories try to link against it. `lib/`
compiles every `.c` file in the directory into an object file and bundles
them into `libtlpi.a` via `ar`. Each program directory compiles its own
`.c` files and links them against `../libtlpi.a`, which it also declares as
a prerequisite so a stale library forces a relink. Shared config from
`Makefile.inc` is included by every sub-Makefile to avoid duplication.

## Key Details

- **Standard**: C99 with `_XOPEN_SOURCE=600`
- **Library**: static (`libtlpi.a`), and every program links directly
  against it. `Makefile.inc` also defines `-lrt`/`-ldl`/`-lacl`/`-lcrypt`/
  `-lcap`/`-pthread` building blocks (`LINUX_LIBRT`, `IMPL_THREAD_FLAGS`,
  etc.), but `LDLIBS` itself is empty — they're only linked in by a
  directory's Makefile that sets `LDLIBS = ${IMPL_LDLIBS} ${LINUX_LIBACL}`
  (or similar) for code that actually needs them, e.g. `threads/` or `acl/`
  from the upstream book tree. Neither `lib` nor `fileio` currently opts in.
- **Programs**: `GEN_EXE` (portable) and `LINUX_EXE` (Linux-specific) lists
  in each directory's Makefile; both are linked the same way.
- **Generated**: `lib/ename.c.inc` is auto-generated from system headers via
  `Build_ename.sh`
- **Compiler**: default is whatever `CC` resolves to (`cc` unless you
  override it). Passing `make CC=clang` additionally enables a few
  clang-only warning suppressions in `Makefile.inc` — there's no probing of
  the actual toolchain, it's a literal string match on `CC`.

## Gotcha to watch for

Executables are declared with `${EXE} : ${TLPI_LIB}` (a prerequisite-only
rule, no recipe). If a name is added to `GEN_EXE`/`LINUX_EXE` before its
matching `.c` file exists, `make <name>` matches that rule, sees `libtlpi.a`
is up to date, and reports "Nothing to be done" instead of failing — it
won't error, it'll just quietly not build. Keep `GEN_EXE`/`LINUX_EXE` in
sync with the `.c` files actually present in the directory.

## Adding New Programs

1. Add `.c` file to an existing directory
2. Add the name to `GEN_EXE` or `LINUX_EXE` in that directory's Makefile

## Adding New Directories

1. Create directory with source files and a Makefile that includes `../Makefile.inc`
2. Add the directory name to `DIRS` in the root Makefile, after `lib`
