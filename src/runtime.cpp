
#include <string>
#include <filesystem>
#include <lua.hpp>
#include "signature.h"

using namespace std;

static int msghandler(lua_State*L)
{
    const char*message=lua_tostring(L, 1);
    if (message==NULL)
    {
        if (luaL_callmeta(L, 1, "__tostring") && lua_type(L, -1)==LUA_TSTRING) return 1;
        message=lua_pushfstring(L, "(error object is a %s value)", luaL_typename(L, 1));
    }
    luaL_traceback(L, L, message, 1);
    return 1;
}

int main(int argc, char*argv[])
{
    const char*progname=argv[0];
    if (lua_State*L=luaL_newstate(); L!=nullptr)
    {
        luaL_openlibs(L);
                                    lua_createtable(L, argc, 0);
                                    for (int i=0; i<argc; i++)
                                    {
                                        lua_pushstring(L, argv[i]);
                                        lua_rawseti(L, -2, i);
                                    }
                                    lua_setglobal(L, "arg");

        luaL_checkstack(L, argc+1, "Out of stack memory (too many arguments to script?).");
        lua_pushcfunction(L, msghandler);

        // Extract the script and push its loader function on to the stack.
        // Use the embedded script name for error messages.
        if (FILE*k=fopen(progname, "rb"); k!=nullptr)
        {
            Signature sig;
            const int sigsize=sizeof sig;
            fseek(k, -sigsize, SEEK_END);
            if (fread(&sig, sigsize, 1, k)!=1)
            {
                fprintf(stderr, "Cannot read signature from '%s'.\n", progname);
                exit(1);
            }
            if (memcmp(sig.sig, SIGNATURE, SIGNATURELEN)!=0)
            {
                fprintf(stderr, "Signature not matched in '%s'.\n", progname);
                exit(1);
            }
            string script(sig.scriptsize, 0);
            fseek(k, sig.runtimesize, SEEK_SET);
            if (const auto numbytes=fread(script.data(), 1, sig.scriptsize, k); numbytes!=sig.scriptsize)
            {
                fprintf(stderr, "Cannot read %ld bytes of code from '%s'.\n", sig.scriptsize, progname);
                exit(1);
            }
            fseek(k, sig.runtimesize+sig.scriptsize, SEEK_SET);
            string scriptname(sig.scriptnamesize,0);
            if (const auto numbytes=fread(scriptname.data(), 1, sig.scriptnamesize, k); numbytes!=sig.scriptnamesize)
            {
                fprintf(stderr, "Cannot read script name from '%s'.\n", progname);
                exit(1);
            }
            fclose(k);
            luaL_loadbufferx(L, script.c_str(), script.size(), scriptname.c_str(), nullptr);
        }
        else
        {
            fprintf(stderr, "Cannot read '%s'\n", progname);
            exit(1);
        }

        // Push further program arguments as arguments to the loader.
        for (int i=1; i<argc; i++) lua_pushstring(L, argv[i]);

        // Execute the loader in protected mode using msghandler.
        if (lua_pcall(L, argc-1, 0, 1)!=0)
        {
            const auto*message=lua_tostring(L, -1);
            fprintf(stderr, "%s\n", message);
            exit(1);
        }

        lua_close(L);

        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Cannot create Lua state: Out of memory.\n");
        exit(1);
    }
}
