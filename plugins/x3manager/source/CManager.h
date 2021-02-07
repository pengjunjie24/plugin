
#pragma once

#include <x3manager/interface/Iplugins.h>
#include <x3framework/ClassMacro.h>

#include <unordered_map>
#include <tuple>
#include <mutex>

namespace x3plugin
{
    typedef bool(*EventDispatcher)(PROC handler, void* data);
    typedef bool(*Creator)(const char*, int64_t, IObject**);
    typedef void(*Destructor)(const char*);

    class IRegister : public IObject
    {
        X3DEFINE_IID(IRegister);
        //注册插件到插件管理器中
        virtual void registerPlugin(Creator creator, Destructor destructor, HMODULE hmod, const char* clsids) = 0;
        //删除插件管理器中插件
        virtual void unregisterPlugin(const char* clsid) = 0;
        //创建其他插件实例
        virtual bool createFromOthers(const std::string& clsid, int64_t iid, IObject** p) = 0;
        //获取插件是否存在
        virtual bool findModuleByFileName(const std::string& filename) = 0;
    };

    class CManager : public IPlugins
        , public IRegister
    {
        X3BEGIN_CLASS_DECLARE(CManager, clsidX3manager)
            X3DEFINE_INTERFACE_ENTRY(IPlugins)
            X3DEFINE_INTERFACE_ENTRY(IRegister)
        X3END_CLASS_DECLARE()

    protected:
        CManager();
        virtual ~CManager();

    private:
        virtual void registerPlugin(Creator creator, Destructor destructor, HMODULE hmod, const char* clsids);
        virtual void unregisterPlugin(const char* clsid);
        virtual bool createFromOthers(const std::string& clsid, int64_t iid, IObject** p);
        virtual bool findModuleByFileName(const std::string& filename);

        virtual int32_t getPluginCount() const;
        virtual void getPluginFiles(std::vector<std::string>& files) const;
        virtual Creator findPluginByClassID(const std::string& clsid) const;

    private:
        enum
        {
            CREATOR_FUNC = 0,
            DESTRUCTOR_FUNC,
            FILE_NAME
        };
        typedef std::unordered_multimap <std::string, std::tuple < Creator, Destructor, std::string >> PluginMap;

        PluginMap _clsPluginmap;//map of clsid and plugin
        mutable std::mutex _registerMutex;
    };
}