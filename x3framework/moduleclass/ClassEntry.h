
#pragma once

#include <x3framework/moduleclass/Iobject.h>

#include <memory>
#include <functional>
#include <string>

namespace x3plugin
{
#if 0
    typedef IObject* (*ObjectCreator)(int64_t iid);
    typedef void (*ObjectDestructor)();
    typedef bool(*HASIID)(int64_t iid);
#endif

    using ObjectCreator = std::function<IObject*(int64_t)>;
    using ObjectDestructor = std::function<void()>;
    using ObjectInit = std::function<bool()>;
    using HASIID = std::function<bool(int64_t)>;

    class ClassEntry
    {
    public:
        ClassEntry(const std::string& className,
                   const std::string& clsid,
                   ObjectCreator creator,
                   ObjectDestructor destructor,
                   ObjectInit initor,
                   HASIID hasiid)
            : _className(className)
            , _clsid(clsid)
            , _creator(creator)
            , _destructor(destructor)
            , _initor(initor)
            , _hasiid(hasiid)
        {
        }

        std::string _className;
        std::string  _clsid;
        ObjectCreator _creator;
        ObjectDestructor _destructor;
        ObjectInit _initor;
        HASIID _hasiid;

        static const ClassEntry s_classEntry;
    };
}