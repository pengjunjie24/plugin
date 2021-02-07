#pragma once

#include <x3framework/ClassMacro.h>
#include <plsimple/interface/isimple.h>
#include <plsimple/interface/isimple2.h>
#include <plsimple/interface/isimple3.h>

#include <vector>

class CSimple
    : public ISimple
    , public ISimple2
    , public ISimple3
{
    X3BEGIN_CLASS_DECLARE(CSimple, plsClsidSimple)
        X3DEFINE_INTERFACE_ENTRY(ISimple)
        X3DEFINE_INTERFACE_ENTRY(ISimple2)
        X3DEFINE_INTERFACE_ENTRY(ISimple3)
    X3END_CLASS_DECLARE()

protected:
    CSimple();
    virtual ~CSimple();

private:
    virtual int add(int a, int b) const;
    virtual int subtract(int a, int b) const;

    virtual int add(const std::vector<int>& nums) const;

    virtual x3plugin::AnyObject createSimple();

public:
    mutable int _sum = 0;
};
