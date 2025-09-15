
# Purpose
Turn a Lua script into an executable.
Inspired by Luiz Henrique de Figueiredo's
    [srlua](https://web.tecgraf.puc-rio.br/~lhf/ftp/lua/index.html).

Further scripts loaded with dofile or require will not be embedded,
but can still be found if `LUA_PATH` or `LUA_CPATH` are set appropriately.

# How to use

    srlua add.lua                   Will create ./add.exe (Windows) or ./add (Linux), embedding add.lua.
    ./add 21 22                     Will execute the embedded script, passing the arguments.

    srlua add.lua other.exe         Will create ./other.exe instead.

    luac -o temp add.lua
    srlua temp add.exe              Will embed bytecode, error messages still refer to the original script.
    ./add a b
    add.lua:5: arg1 ('a') is not a number!
    stack traceback:
            [C]: in function 'error'
            add.lua:5: in main chunk

# Improvements over the original

- No need for a second executable, srlua is a standalone solution.
- Improved error messages from embedded scripts by embedding the script name along with its content.
- Rewritten in C++
- Windows build

# How to build

## on Windows

    Edit buildsys/VS17/Lua.props    Set AdditionalIncludeDirectories, AdditionalDependencies
                                    and AdditionalLibraryDirectories according to your
                                    Lua-Installation.
    Open buildsys/VS17/SRLua.sln    Build Configuration Release/32bit
                                    Release/32 bits will be built in repository root.

## or on Linux
Use Makefile

    make                            Will make srlua and compile add.lua to ./add
