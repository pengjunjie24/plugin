#include <plsimple/source/plsimple.h>
#include <x3framework/PluginImpl.h>

CSimple::CSimple()
{
}

CSimple::~CSimple()
{
}

int CSimple::add(int a, int b) const
{
    return a + b;
}

int CSimple::subtract(int a, int b) const
{
    return a - b;
}

int CSimple::add(const std::vector<int>& nums) const
{
    int ret = 0;

    for (std::vector<int>::const_iterator it = nums.begin();
        it != nums.end(); ++it)
    {
        ret += *it;
    }

    return ret;
}

x3plugin::AnyObject CSimple::createSimple()
{
    return x3plugin::Object<ISimple>(plsClsidSimple);
}
