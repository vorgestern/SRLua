
# Purpose
Turn a Lua script into an executable.
Inspired by Luiz Henrique de Figueiredo's
    [srlua](https://web.tecgraf.puc-rio.br/~lhf/ftp/lua/index.html).

Further scripts loaded with dofile or require will not be embedded,
but can still be found if `LUA_PATH` or `LUA_CPATH` are set appropriately.

# How to use

    srlua add.lua                   Will create ./add.exe, embedding add.lua.
    ./add 21 22                     Will execute the embedded script, passing the arguments.

    srlua add.lua other.exe         Will create ./other.exe instead.

# Improvements over the original

- No need for a second executable, srlua is a standalone solution.
- Improved error messages from embedded scripts by embedding the script name along with its content.
- Rewritten in C++
- Windows build

# How to build

## on Windows

    Use buildsys/VS17/SRLua.sln     Release/32 bits will be built in repository root.

## or on Linux
Use Makefile

    make                            Will make srlua and compile add.lua to ./add
