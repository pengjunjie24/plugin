
#pragma once

#include <x3framework/moduleclass/PluginDef.h>

//接口注册定义
#define X3BEGIN_CLASS_DECLARE(Cls, clsid)  \
public: \
    static const char* _getClassId() { return clsid; }   \
    static const char* _getClassName() { return #Cls; }   \
    static bool _queryObject(const Cls* self, int64_t iid, x3plugin::IObject** p) \
    {

#define X3DEFINE_INTERFACE_ENTRY(_Interface)    \
        if (iid == _Interface::getIID())        \
        {   \
            if (self) {     \
                *p = (x3plugin::IObject*)(_Interface*)(self); \
                (*p)->retainObject(); \
            }   \
            return true;    \
        }

#define X3USE_INTERFACE_ENTRY(_BaseClass)   \
        if (_BaseClass::_queryObject(self, iid, p))   \
        {   \
            return true;    \
        }

#define X3END_CLASS_DECLARE() \
        return false; \
    }   \
protected:  // Ensure the following constructor is protected.
