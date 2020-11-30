
#include <pltempl/source/pltempl.h>
#include <x3framework/PluginImpl.h>

CSimple::CSimple()
{
    printf("plt CSimple construct\n");
}

CSimple::~CSimple()
{
    printf("plt ~CSimple distruct\n");
}

int CSimple::add(int a, int b) const
{
    return a + b;
}

int CSimple::subtract(int a, int b) const
{
    return a - b;
}
