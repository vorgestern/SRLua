
#define SIGNATURE "%%srglue"
#define SIGNATURELEN (sizeof(SIGNATURE)-1)

struct Signature
{
    char sig[SIGNATURELEN+1];
    size_t runtimesize, scriptsize, scriptnamesize;
};
