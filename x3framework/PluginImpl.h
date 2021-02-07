
#pragma once

#include <x3framework/ClassEntry.h>
#include <x3framework/PluginDef.h>
#include <x3framework/Iobject.h>
#include <x3framework/ModuleAction.h>

#include <assert.h>
#include <string.h>

//加载动态库后在进程中所对应的空间
namespace x3plugin
{
    static HMODULE s_hmod = NULL;//插件操作句柄
    static HMODULE s_hmanager = NULL;//插件管理器
    static std::string s_filename = "";//插件所在路径

    OUTAPI bool x3InitializePlugin();//模块加载后调用的初始化函数
    OUTAPI void x3UninitializePlugin();//模块卸载后调用的析构函数

    //获取当前插件空间中的插件实例id
    static const char* getClassId()
    {
        const ClassEntry cls = ClassEntry::s_classEntry;
        assert(cls._creator);//插件实体必须初始化
        return cls._clsid.c_str();
    }

    //通过iid获取clsid
    static bool getDefaultClassId(const int64_t& iid, const char*& clsid)
    {
        const ClassEntry cls = ClassEntry::s_classEntry;
        if (cls._hasiid(iid))
        {
            clsid = cls._clsid.c_str();
            return true;
        }
        return false;
    }

    //在当前动态库空间内创建对象
    //在x3manager中调用
    OUTAPI bool x3InternalCreate(const char* clsid, int64_t iid, IObject** p)
    {
        *p = NULL;
        if (0 == *clsid)
        {
            getDefaultClassId(iid, clsid);
        }

        const ClassEntry cls = ClassEntry::s_classEntry;
        if (cls._clsid == clsid)
        {
            *p = cls._creator(iid);
            return *p != NULL;
        }

        return false;
    }

    OUTAPI void x3InternalDestruct(const char* clsid)
    {
        const ClassEntry cls = ClassEntry::s_classEntry;
        if (cls._clsid == clsid && cls._destructor)
        {
            cls._destructor();
        }
    }

    OUTAPI bool x3InitPlugin(HMODULE hmod,
        HMODULE hmanager, const char* filename)
    {
        assert(hmod && hmanager);

        //s_hmod和s_hmanager存在能确认该插件已被初始化
        if (s_hmod && s_hmanager)
        {
            return true;
        }

        s_hmod = hmod;
        s_hmanager = hmanager;
        s_filename = filename;

        bool needInit = true;
        if (s_hmanager != s_hmod)
        {
            typedef bool (*CreateFunc)(const char*, int64_t, IObject**);//创建插件方法
            typedef void (*DestructFunc)(const char*);//删除插件方法
            typedef bool (*RegisterFunc)(CreateFunc, DestructFunc, HMODULE, const char*);//注册插件方法

            RegisterFunc freg = (RegisterFunc)ModuleAction::getProcAddress(
                s_hmanager, "x3RegisterPlugin");

            needInit = !freg || freg(x3InternalCreate, x3InternalDestruct, s_hmod, getClassId());
        }

        return !needInit || x3InitializePlugin();
    }

    OUTAPI bool x3FreePlugin()
    {
        assert(s_hmanager);

        bool needFree = true;
        if (s_hmanager && s_hmanager != s_hmod)
        {
            assert(s_hmod);

            typedef bool(*UnregisterFunc)(const char*);

            UnregisterFunc unregisterfunc = (UnregisterFunc)
                ModuleAction::getProcAddress(s_hmanager, "x3UnregisterPlugin");
            needFree = !unregisterfunc || unregisterfunc(getClassId());
        }

        if(needFree)
        {
            x3UninitializePlugin();
        }

        return true;
    }

    OUTAPI bool getdlname(char* filename, int16_t size)
    {
        return !!strncpy(filename, s_filename.c_str(), size);
    }

    //创建插件对象实例
    LOCALAPI bool createObject(const char* clsid, int64_t iid, IObject** p)
    {
        if (!x3InternalCreate(clsid, iid, p) && s_hmanager)
        {
            typedef bool(*CreateFunc)(const char*, int64_t, IObject**);
            CreateFunc createFunc = (CreateFunc)ModuleAction::getProcAddress(
                s_hmanager, "x3CreateObject");
            return createFunc && createFunc(clsid, iid, p);
        }
        return *p != NULL;
    }

    LOCALAPI HMODULE getModuleHandle()
    {
        return s_hmod;
    }

    LOCALAPI HMODULE getManagerModule()
    {
        return s_hmanager;
    }
}