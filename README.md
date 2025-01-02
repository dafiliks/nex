# Nex Lang
> [!NOTE]
> This project is currently in WIP.

Nex Lang is an experimental (toy) programming language written in C++, that was made as a learning project. The purpose of this language is not to be the fastest or the most functional but to serve as an example of how a programming language is created.

I undertook this project as I was interested in how compilers work, and so far the experience has been rewarding and interesting. Although it was very tempting, I have decided to not continue using LLVM for code generation as I wanted to use this project to get better at reading and writing assembly code. I am trying to build this project entirely from scratch, therefore, most things will be found out through trial and error, so don't expect the implementation to be perfect.

# Info

### Assembly
Currently, Nex only outputs Intel x64 assembly. You can execute the assembly spat out by Nex using nasm.

*Install commands for Arch Linux:*
```bash
sudo pacman -S nasm
```

To execute the assembly:

```bash
nasm -felf64 out.asm
ld -o out out.o
```

### Extention
`.nex` for obvious reasons.

# Building

#### Linux:
Building Nex on Linux requires you to have CMake, Make, and GCC.

*Install commands for Arch Linux:*
```bash
sudo pacman -S cmake base-devel
```

To build Nex:

```bash
git clone https://github.com/dafiliks/nex-lang
mkdir nex/build
cd nex/build
cmake ..
make
# You can execute Nex with the commands below
cd compiler/src
./nex
```

# Contributing

All contributions are welcome, so feel free to submit issues/pull requests. Coding style doesn't matter as long as naming is clear. Programming is art.

<sub> © David Filiks </sub>
