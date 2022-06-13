# Introduction
This tool converts file in the M68K ELF32 format to the [FoenixMCP](https://github.com/pweingar/FoenixMCP) PGZ format.

# Installation
Given the prerequisites (mainly `cmake` and [just](https://github.com/casey/just)) are installed, `elf2pgz` can be installed in `$HOME/.local/bin` by running

```
just install
```

# Usage
To convert an M68K ELF32 executable to FoenixMCP PGZ format, the input and output file names must be given on the command line:

```
$ elf2pgz INFILE OUTFILE
```
