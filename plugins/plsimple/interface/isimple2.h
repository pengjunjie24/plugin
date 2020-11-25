#pragma once

#include <x3framework/Objptr.hpp>
#include <vector>

class ISimple2 : public x3plugin::IObject
{
    X3DEFINE_IID(ISimple2);
    virtual int add(const std::vector<int>& nums) const = 0;
};