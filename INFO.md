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
Nex Lang compiles into x64 Intel assembly language.

# Building

Building Nex on Linux requires you to have CMake, Make, and GCC.

To build Nex:

```bash
git clone https://github.com/dafiliks/nex-lang
mkdir nex-lang/build
cd nex-lang/build
cmake ..
make
cd compiler/src
./nex # Debug mode
```

# Contributing

All contributions are welcome, so feel free to submit issues/pull requests.

<sub> © David Filiks </sub>
