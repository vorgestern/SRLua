
#define SIGNATURE "%%srglue"
#define SIGNATURELEN (sizeof(SIGNATURE)-1)

// Executable file structure:
// runtime | script | scriptname | signature | << end

// Offsets:
// runtime          0
// script           signature.runtimesize
// scriptname       signature.runtimesize+signature.scriptsize
// signature        filesize-sizeof Signature 

struct Signature
{
    char sig[SIGNATURELEN+1];
    size_t runtimesize, scriptsize, scriptnamesize;
};
