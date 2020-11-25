#include <x3framework/ModuleMacro.h>

#include <plsimple/source/plsimple.h>

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY_SINGLETON(CSimple)
XEND_DEFINE_MODULE()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}
