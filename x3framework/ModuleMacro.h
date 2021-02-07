
#pragma once

#include <x3framework/ClassEntry.h>
#include <x3framework/SingletonObject.hpp>
#include <x3framework/NormalObject.hpp>

//插件实体注册定义

#define XBEGIN_DEFINE_MODULE()  \
    const x3plugin::ClassEntry x3plugin::ClassEntry::s_classEntry =

#define XDEFINE_CLASSMAP_ENTRY(cls) \
    x3plugin::ClassEntry("NormalObject<"#cls">", cls::_getClassId(),  \
        (x3plugin::ObjectCreator)(&x3plugin::NormalObject<cls>::create), NULL, \
        (x3plugin::HASIID)(&x3plugin::NormalObject<cls>::hasInterface));

#define XDEFINE_CLASSMAP_ENTRY_SINGLETON(cls)    \
    x3plugin::ClassEntry("SingletonObject<"#cls">", cls::_getClassId(),  \
        (x3plugin::ObjectCreator)(&x3plugin::SingletonObject<cls>::create), \
        (x3plugin::ObjectDestructor)(&x3plugin::SingletonObject<cls>::deleteObject), \
        (x3plugin::HASIID)(&x3plugin::SingletonObject<cls>::hasInterface));

#define XEND_DEFINE_MODULE()
