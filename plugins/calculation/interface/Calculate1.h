#pragma once

#include <x3framework/Objptr.hpp>

const char* const c_calculationClsid = "94071767-ba6b-4769-9eb4-2ebf469289f3";

class Calculate1 : public x3plugin::IObject
{
    X3DEFINE_IID(Calculate1);
    virtual int add(int a, int b) const = 0;
    virtual int subtract(int a, int b) const = 0;
};
