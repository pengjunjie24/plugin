#include <x3framework/ModuleMacro.h>

#include <plsimple/source/plsimple.h>

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY_SINGLETON(CSimple)
XEND_DEFINE_MODULE()

OUTAPI bool x3InitializePlugin()
{
    printf("pls module init\n");
    return true;
}

OUTAPI void x3UninitializePlugin()
{
    printf("pls module un init\n");
}
