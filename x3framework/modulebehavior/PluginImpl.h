
#pragma once

#include <x3framework/moduleclass/ClassMacro.h>
#include <x3framework/moduleclass/PluginDef.h>
#include <x3framework/moduleclass/Iobject.h>

#include <assert.h>
#include <string.h>

//加载动态库后在进程中所对应的空间
namespace x3plugin
{
    static HMODULE s_hmod = NULL;//插件操作句柄
    static std::string s_clsid = "";//插件唯一标识

    //获取当前插件空间中的插件实例id
    static const char* getClassId()
    {
        const ClassEntry& cls = ClassEntry::s_classEntry;
        assert(cls._creator);//插件实体必须初始化
        return cls._clsid.c_str();
    }

    //通过iid获取clsid
    static bool getDefaultClassId(const int64_t& iid, const char*& clsid)
    {
        const ClassEntry& cls = ClassEntry::s_classEntry;
        if (cls._hasiid(iid))
        {
            clsid = cls._clsid.c_str();
            return true;
        }
        return false;
    }

    //在当前动态库空间内创建对象
    OUTAPI bool x3InternalCreate(const char* clsid, int64_t iid, IObject** p)
    {
        *p = NULL;

        if (0 == *clsid)
        {
            getDefaultClassId(iid, clsid);
        }

        const ClassEntry& cls = ClassEntry::s_classEntry;
        if (cls._clsid == clsid)
        {
            *p = cls._creator(iid);
            return *p != NULL;
        }

        return false;
    }

    //在当前动态库中释放创建资源
    bool x3InternalDestruct()
    {
        const ClassEntry& cls = ClassEntry::s_classEntry;
        cls._destructor();
        return true;
    }

    OUTAPI bool x3InitPlugin(HMODULE hmod, const char*& chclsid)
    {
        assert(hmod);
        if (!hmod)
        {
            fprintf(stderr, "x3InitPlugin hmod is null\n");
            return false;
        }

        if (s_hmod)
        {
            fprintf(stderr, "x3InitPlugin s_hmod has been inited\n");
            return false;
        }

        s_hmod = hmod;
        chclsid = getClassId();
        const ClassEntry& cls = ClassEntry::s_classEntry;
        return cls._initor();
    }

    OUTAPI bool x3FreePlugin()
    {
        assert(s_hmod);
        if (!s_hmod)
        {
            fprintf(stderr, "x3FreePlugin has been free\n");
            return false;
        }

        return x3InternalDestruct();
    }

    bool createObject(const char*clsid, int64_t iid, IObject** point)
    {
        return x3InternalCreate(clsid, iid, point);
    }

}