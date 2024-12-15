
/*
* srglue.c
* glue exe and script for srlua
* Luiz Henrique de Figueiredo <lhf@tecgraf.puc-rio.br>
* 13 Aug 2019 08:44:57
* This code is hereby placed in the public domain and also under the MIT license
*/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string_view>
#include "srglue.h"

using namespace std;

static const char*progname="srglue";

static void cannot(const char*what, const char*name)
{
    fprintf(stderr, "%s: cannot %s %s: %s\n", progname, what, name, strerror(errno));
    exit(EXIT_FAILURE);
}

static FILE*open(const char*name, const char*mode, const char*outname)
{
    if (outname!=NULL && strcmp(name, outname)==0)
    {
        errno=EPERM;
        cannot("overwrite input file", name);
        return NULL;
    }
    else
    {
        FILE*f=fopen(name, mode);
        if (f==NULL) cannot("open", name);
        return f;
    }
}

static long copy(FILE*in, const char*name, FILE*out, const char*outname)
{
    long size;
    if (fseek(in, 0, SEEK_END)!=0) cannot("seek", name);
    size=ftell(in);
    if (fseek(in, 0, SEEK_SET)!=0) cannot("seek", name);
    for (;;)
    {
        char b[BUFSIZ];
        int n=fread(&b, 1, sizeof(b), in);
        if (n==0) { if (ferror(in)) cannot("read", name); else break; }
        if (fwrite(&b, n, 1, out)!=1)  cannot("write", outname);
    }
    fclose(in);
    return size;
}

static long copy(string_view in, const char*name, FILE*out, const char*outname)
{
    const long size=in.size();
    if (fwrite(in.data(), size, 1, out)!=1)  cannot("write", outname);
    return size;
}

string_view chunk_runtime();

int main(int argc, char*argv[])
{
    if (argv[0]!=NULL && argv[0][0]!=0) progname=argv[0];
    if (argc!=3)
    {
        fprintf(stderr, "usage: %s in.lua out.exe\n", progname);
        return 1;
    }
    else
    {
        const char*scriptname=argv[1];
        const char*outname=argv[2];
        string_view runtime=chunk_runtime();
        FILE*in2=open(argv[1], "rb", argv[3]);
        FILE*out=open(argv[2], "wb", NULL);
        Glue t={GLUESIG, 0, 0};
        t.size1=copy(runtime, "runtime", out, outname);
        t.size2=copy(in2, scriptname, out, outname);
        printf("Runtime: %u Bytes (%u)\n", runtime.size(), t.size1);
        printf("Script: %u Bytes (%u)\n", runtime.size(), t.size2);
        if (fwrite(&t, sizeof(t), 1, out)!=1) cannot("write", outname);
        if (fclose(out)!=0) cannot("close", outname);
        return 0;
    }
}
