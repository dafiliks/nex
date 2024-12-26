# Nex Lang
> [!NOTE]
> This project is currently in WIP.

Nex Lang is an experimental programming language written in C++, that was made as a learning project. The purpose of this language is not to be the fastest or the most functional but to serve as an example of how a programming language may be created.

I undertook this project as I was interested in how compilers work, and so far the experience has been eye-opening. I am currently lexing and parsing myself and using LLVM to generate IR for the backend. I've decided to use LLVM for this project as I do not want Nex to be insanely slow. Writing many optimizations and getting ASM generation working on different architectures is not appealing or exciting for me.

# Building

#### Linux:
Building Nex on Linux requires you to have CMake, Make, GCC, and LLVM.

*Install commands for Arch Linux:*
```bash
sudo pacman -S cmake base-devel llvm
```

Now that you have all the dependencies installed, it's time to build Nex.

```bash
git clone https://github.com/dafiliks/nex
mkdir nex/build
cd nex/build
cmake ..
make
# You can execute Nex with the commands below
cd src
./nex
```

# Contributing

All contributions are welcome, so feel free to submit issues/pull requests. Coding style doesn't matter as long as naming is clear. Programming is art.

<sub> © David Filiks </sub>
