
#ifdef _MSC_BUILD

#include <Windows.h>
#include <string_view>

using std::string_view;

static string_view loadresource_msvc(unsigned resid, unsigned restype)
{
    HRSRC resinfo=FindResource(nullptr, MAKEINTRESOURCE(resid), MAKEINTRESOURCE(restype));
    if (resinfo!=0)
    {
        HGLOBAL reshandle=LoadResource(nullptr, resinfo);
        if (reshandle!=nullptr) return {static_cast<char*>(LockResource(reshandle)),
            static_cast<size_t>(SizeofResource(nullptr, resinfo))};
        else return {};
    }
    else return {};
}

string_view chunk_runtime() { return loadresource_msvc(101, 1024); }

#endif
