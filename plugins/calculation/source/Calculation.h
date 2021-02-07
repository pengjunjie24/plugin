#pragma once

#include <x3framework/InterfaceMacro.h>
#include <calculation/interface/Calculate1.h>
#include <calculation/interface/Calculate2.h>
#include <calculation/interface/Creator.h>

#include <vector>

class Calculation
    : public Calculate1
    , public Calculate2
    , public Creation
{
    X3BEGIN_CLASS_DECLARE(Calculation, c_calculationClsid)
        X3DEFINE_INTERFACE_ENTRY(Calculate1)
        X3DEFINE_INTERFACE_ENTRY(Calculate2)
        X3DEFINE_INTERFACE_ENTRY(Creation)
    X3END_CLASS_DECLARE()

protected:
    Calculation();
    virtual ~Calculation();

private:
    virtual int add(int a, int b) const;
    virtual int subtract(int a, int b) const;

    virtual int add(const std::vector<int>& nums) const;

    virtual x3plugin::AnyObject createSimple();

public:
    mutable int _sum = 0;
};
