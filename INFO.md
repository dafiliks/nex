# Info

### Commands
Below, you can see the usage of the Nex executable and some options. You can bring this info up for yourself by running `nex help`.

```
Nex: Usage: nex <file.nex> <file.asm>
nex version         Displays all version information
nex                 Runs Nex in debug mode
```


### Assembly
You can use Nasm to assemble the output:

```bash
nasm -felf64 file.asm
ld -o file file.o
```

### Extention
`.nex` for obvious reasons.

### Output Target
Nex Lang compiles into x86-64 Intel style assembly language.

### Compatibility
Currently, this project has been developed and tested on Linux (Arch) machines only.

# Building

Building Nex on Linux requires you to have CMake, Make, and GCC.

To build Nex:

```bash
git clone https://github.com/dafiliks/nex
mkdir nex/build
cd nex/build
cmake ..
make
cd compiler/src
./nex # Debug mode
```

When you run `make` after using `cmake ..`, you might get warnings. These warnings are false positives as the compiler does not see that the error functions are assert wrappers.

# Contributing

All contributions are welcome, so feel free to submit issues/pull requests.

<sub> © David Filiks </sub>
