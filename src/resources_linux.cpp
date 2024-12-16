
#include <cstring>
#include <string_view>

using std::string_view;

// Die Adressen dieser Symbole liegen in b/runtime.o,
// wo sie von objcopy erzeugt wurden (beim Bauen unter Linux).
// Beachte die Anpassung der Namen (opjcopy --redefine-sym=...)
extern "C" char runtime_start;
extern "C" char runtime_end;

string_view chunk_runtime()
{
    return {&runtime_start, static_cast<size_t>(&runtime_end-&runtime_start)};
}
