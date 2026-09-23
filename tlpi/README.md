# TLPI - The Linux Programming Interface

Code exercises from [The Linux Programming Interface](https://man7.org/tlpi/).

## Build

```bash
make all
```

See [BUILD.md](BUILD.md) for full build system documentation.

## Regenerating `compile_commands.json`

Do not edit `compile_commands.json` by hand. Use [Bear](https://github.com/rizsotto/Bear) to regenerate it:

```bash
bear -- make clean && make
```

Run this whenever you add or remove `.c` files.
