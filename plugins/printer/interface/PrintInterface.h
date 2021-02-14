#pragma once

#include <x3framework/moduleclass/Objptr.hpp>

const char* const c_printClsid = "78d30c77-e0f0-48a3-a489-dd4327759c27";

class PrintInterface : public x3plugin::IObject
{
    X3DEFINE_IID(PrintInterface);
    virtual int add(int a, int b) const = 0;
    virtual int subtract(int a, int b) const = 0;
};
