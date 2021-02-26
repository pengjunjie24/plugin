
#pragma once

#include <stdint.h>

namespace x3plugin
{
    inline int64_t hashkey(const char* strHash)
    {
        uint64_t hashValue = 0;
        while (*strHash)
        {
            hashValue = (hashValue << 5) + hashValue + *strHash++;
        }
        return hashValue;
    }

// IID is unique only in a implement class of a plugin.
#define X3DEFINE_IID(_Interface)    \
public: \
    virtual ~_Interface() {}   \
    static const char* getInterfaceName() { return #_Interface; }   \
    static int64_t getIID() { static int64_t iid = x3plugin::hashkey(#_Interface);    \
        return iid; }

    //所有插件的基类
    class IObject
    {
        X3DEFINE_IID(IObject);

        virtual int64_t retainObject() const = 0;
        virtual int64_t releaseObject() const = 0;
        virtual bool queryObject(int64_t iid, IObject** point) const = 0;

        virtual const char* getClassId() const = 0;
        virtual const char* getClassName() const = 0;

        virtual bool init() = 0;
    };

}