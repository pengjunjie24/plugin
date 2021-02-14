
#include <printer/source/PrintImpl.h>
#include <x3framework/modulebehavior/PluginImpl.h>

PrintImpl::PrintImpl()
{
    printf("PrintImpl::PrintImp construct\n");
}

PrintImpl::~PrintImpl()
{
    printf("PrintImpl::~PrintImpl distruct\n");
}

int PrintImpl::add(int a, int b) const
{
    return a + b;
}

int PrintImpl::subtract(int a, int b) const
{
    return a - b;
}
