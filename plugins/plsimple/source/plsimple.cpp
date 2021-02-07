#include <plsimple/source/plsimple.h>
#include <x3framework/PluginImpl.h>

CSimple::CSimple()
{
    printf("PLS::CSimple() construct\n");
}

CSimple::~CSimple()
{
    printf("~PLS::CSimple() disstruct\n");
}

int CSimple::add(int a, int b) const
{
    _sum += (a + b);
    printf("add _sum = %d\n", _sum);
    return a + b;
}

int CSimple::subtract(int a, int b) const
{
    _sum += (a - b);
    printf("subtract _sum = %d\n", _sum);
    return a - b;
}

int CSimple::add(const std::vector<int>& nums) const
{
    int ret = 0;

    for (std::vector<int>::const_iterator it = nums.begin();
        it != nums.end(); ++it)
    {
        _sum += *it;
        ret += *it;
    }

    printf("add vector _sum = %d\n", _sum);
    return ret;
}

x3plugin::AnyObject CSimple::createSimple()
{
    return x3plugin::Object<ISimple>(plsClsidSimple);
}
