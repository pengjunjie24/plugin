#pragma once

#include <x3framework/ClassMacro.h>
#include <pltempl/interface/pltisimple.h>

class CSimple : public PltISimple
{
    X3BEGIN_CLASS_DECLARE(CSimple, IsClsidSimple)
        X3DEFINE_INTERFACE_ENTRY(PltISimple)
    X3END_CLASS_DECLARE()
protected:
    CSimple();
    virtual ~CSimple();

private:
    virtual int add(int a, int b) const;
    virtual int subtract(int a, int b) const;
};