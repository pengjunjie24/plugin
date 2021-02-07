#pragma once

#include <x3framework/Objptr.hpp>
#include <vector>

class Calculate2 : public x3plugin::IObject
{
    X3DEFINE_IID(Calculate2);
    virtual int add(const std::vector<int>& nums) const = 0;
};