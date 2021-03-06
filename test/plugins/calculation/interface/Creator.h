#pragma once

#include <x3framework/moduleclass/Objptr.hpp>

const char* const clsidSimple3 = "94071767-ba6b-4769-9eb4-2ebf469218f3";

class Creation : public x3plugin::IObject
{
    X3DEFINE_IID(Creation);

    virtual x3plugin::AnyObject createSimple() = 0;
};