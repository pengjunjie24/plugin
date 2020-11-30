
#pragma once

#include <x3manager/interface/Iplugins.h>
#include <x3framework/ClassMacro.h>

#include <unordered_map>
#include <map>
#include <mutex>

namespace x3plugin
{
    typedef bool(*EventDispatcher)(PROC handler, void* data);
    typedef bool(*Creator)(const char*, int64_t, IObject**);

    class IRegister : public IObject
    {
        X3DEFINE_IID(IRegister);
        //注册插件到插件管理器中
        virtual void registerPlugin(Creator creator, HMODULE hmod, const char** clsids) = 0;
        //删除插件管理器中插件
        virtual void unregisterPlugin(Creator creator) = 0;
        //创建其他插件实例
        virtual bool createFromOthers(const std::string& clsid, int64_t iid, IObject** p) = 0;
        //通过插件名获取插件
        virtual HMODULE findModuleByFileName(const std::string& filename) = 0;
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
        virtual void registerPlugin(Creator creator, HMODULE hmod, const char** clsids);
        virtual void unregisterPlugin(Creator creator);
        virtual bool createFromOthers(const std::string& clsid, int64_t iid, IObject** p);
        virtual HMODULE findModuleByFileName(const std::string& filename);

        virtual int32_t getPluginCount() const;
        virtual void getPluginFiles(std::vector<std::string>& files) const;
        virtual Creator findPluginByClassID(const std::string& clsid) const;

    private:
        typedef std::pair<Creator, HMODULE> Plugin;//插件创建函数和插件句柄的映射
        typedef std::unordered_multimap <std::string, Creator> CreatorMap;//插件实例和创建函数映射
        typedef std::pair<std::string, Creator> CreatorPair;

        std::vector<Plugin> _plugins;
        CreatorMap _clsmap;//map of clsid and creatorFunc
        mutable std::mutex _registerMutex;
    };
}