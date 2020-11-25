#pragma once

#include <x3framework/Objptr.hpp>

const char* const IsClsidSimple = "78d30c77-e0f0-48a3-a489-dd4327759c27";

class ISimple : public x3plugin::IObject
{
    X3DEFINE_IID(ISimple);
    virtual int add(int a, int b) const = 0;
    virtual int subtract(int a, int b) const = 0;
};
