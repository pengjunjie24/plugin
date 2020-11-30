#include <x3framework/ModuleMacro.h>

#include <pltempl/source/pltempl.h>

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY_SINGLETON(CSimple)
XEND_DEFINE_MODULE()

OUTAPI bool x3InitializePlugin()
{
    printf("plt module init\n");
    return true;
}

OUTAPI void x3UninitializePlugin()
{
    printf("plt module un init\n");
}
