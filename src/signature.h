
#define SIGNATURE "%%srglue"
#define SIGNATURELEN (sizeof(SIGNATURE)-1)

struct Signature
{
    char sig[SIGNATURELEN+1];
    long runtimesize, scriptsize, scriptnamesize;
};
