
#pragma once

#include <x3framework/Iobject.h>

#include <memory>
#include <string>

namespace x3plugin
{
    typedef IObject* (*ObjectCreator)(int64_t iid);
    typedef bool(*HASIID)(int64_t iid);

    class ClassEntry
    {
    public:
        //typedef std::shared_ptr<ClassEntry> sptr;

        ClassEntry(const std::string& className,
                   const std::string& clsid,
                   ObjectCreator creator,
                   HASIID hashiid)
            : _className(className)
            , _clsid(clsid)
            , _creator(creator)
            , _hasiid(hashiid)
        {
        }

        ClassEntry()
            : _className("")
            , _clsid("")
            , _creator(NULL)
            , _hasiid(NULL)
        {
        }

        std::string _className;
        std::string  _clsid;
        ObjectCreator _creator;
        HASIID _hasiid;

        //static const sptr s_classes[];
        static const ClassEntry s_classes[];
    };
}