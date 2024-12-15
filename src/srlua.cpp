
#include <string>
#include <string_view>
#include "signature.h"

using namespace std;

string_view chunk_runtime();

string filecontent(const string&f)
{
    FILE*k=fopen(f.c_str(), "rb");
    if (k==nullptr) return {};
    const int size=(fseek(k, 0, SEEK_END), ftell(k));
    fseek(k, 0, SEEK_SET);
    string result(size,0);
    if (fread(result.data(), 1, size, k)!=size) return {};
    return result;
}

int main(int argc, char*argv[])
{
    string fnscript, fnexec;
    for (auto a=1; a<argc; ++a)
    {
        if (fnscript.empty()) fnscript=argv[a];
        else if (fnexec.empty()) fnexec=argv[a];
    }
    if (fnscript.empty())
    {
        fprintf(stderr, R"__(Usage: %s luascript [execfile]
will write the executable version of luascript to execfile.
If execfile is not given, it will be derived from scriptname.)__", argv[0]);
        exit(1);
    }
    if (fnexec.empty())
    {
#ifdef _MSC_BUILD
        fnexec=string(fnscript, 0, fnscript.rfind(".lua"))+".exe";
#else
        fnexec=string(fnscript, 0, fnscript.rfind(".lua"));
#endif
    }

    if (const auto scripttext=filecontent(fnscript); !scripttext.empty())
    {
        string_view runtime=chunk_runtime();
        const Glue signature={GLUESIG, static_cast<long>(runtime.size()), static_cast<long>(scripttext.size())};

        // printf("script: '%s' (%u)\n", fnscript.c_str(), scripttext.size());
        // printf("exec:   '%s'\n", fnexec.c_str());
        // return 0;

        if (FILE*kexec=fopen(fnexec.c_str(), "wb"); kexec!=nullptr)
        {
            int rc=0;
            if (const auto len1=fwrite(runtime.data(), 1, runtime.size(), kexec); len1!=runtime.size())
                rc=fprintf(stderr, "Failed to write runtime to output file '%s'\n", fnexec.c_str()),1;
            if (const auto len2=fwrite(scripttext.c_str(), 1, scripttext.size(), kexec); len2!=scripttext.size())
                rc=fprintf(stderr, "Failed to write script source to output file '%s'\n", fnexec.c_str()),1;
            if (const auto len3=fwrite(&signature, 1, sizeof(signature), kexec); len3!=sizeof(signature))
                rc=fprintf(stderr, "Failed to signature to output file '%s'\n", fnexec.c_str()),1;
            fclose(kexec);
            return rc;
        }
        else return fprintf(stderr, "Failed to create output file '%s'\n", fnexec.c_str()),1;
    }
    else return fprintf(stderr, "Failed to read script '%s'\n", fnscript.c_str()),1;
}
