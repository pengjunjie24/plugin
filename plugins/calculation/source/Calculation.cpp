#include <calculation/source/Calculation.h>
#include <x3framework/modulebehavior/PluginImpl.h>

Calculation::Calculation()
{
    printf("Calculation::Calculation() construct\n");
}

Calculation::~Calculation()
{
    printf("Calculation::~Calculation() disstruct\n");
}

int Calculation::add(int a, int b) const
{
    _sum += (a + b);
    printf("add _sum = %d\n", _sum);
    return a + b;
}

int Calculation::subtract(int a, int b) const
{
    _sum += (a - b);
    printf("subtract _sum = %d\n", _sum);
    return a - b;
}

int Calculation::add(const std::vector<int>& nums) const
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

x3plugin::AnyObject Calculation::createSimple()
{
    return x3plugin::Object<Calculate1>(c_calculationClsid);
}
