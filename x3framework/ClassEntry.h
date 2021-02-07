
#pragma once

#include <x3framework/Iobject.h>

#include <memory>
#include <string>

namespace x3plugin
{
    typedef IObject* (*ObjectCreator)(int64_t iid);
    typedef void (*ObjectDestructor)();
    typedef bool(*HASIID)(int64_t iid);

    class ClassEntry
    {
    public:
        ClassEntry(const std::string& className,
                   const std::string& clsid,
                   ObjectCreator creator,
                   ObjectDestructor destructor,
                   HASIID hasiid)
            : _className(className)
            , _clsid(clsid)
            , _creator(creator)
            , _destructor(destructor)
            , _hasiid(hasiid)
        {
        }

        std::string _className;
        std::string  _clsid;
        ObjectCreator _creator;
        ObjectDestructor _destructor;
        HASIID _hasiid;

        static const ClassEntry s_classEntry;
    };
}