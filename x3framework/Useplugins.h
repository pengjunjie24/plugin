
#pragma once

#include <x3framework/ModuleAction.h>

#include <vector>

namespace x3plugin
{
    static std::vector<ModuleAction::uptr> s_plugins;

    class IObject;
    bool createObject(const char* clsid, int64_t iid, IObject** p)
    {
        typedef bool(*createFunc)(const char*, int64_t, IObject**);

        for (const auto& plugin : s_plugins)
        {
            if (clsid == plugin->getClsid())
            {
               createFunc func = (createFunc)plugin->getFunc("x3InternalCreate");
               return func && func(clsid, iid, p);
            }
        }
        return false;
    }

    //插件加载和卸载(用户调用)
    class AutoLoadPlugins: Noncopyable
    {
    public:
        //pluginsName: 插件名字
        //folder: 插件路径
        AutoLoadPlugins(const std::vector<std::string>& pluginsName,
            const std::string& folder = "")
        {
            loadPlugins(pluginsName, folder);
        }

        ~AutoLoadPlugins()
        {
            unloadPlugins();
        }

        static int16_t getPluginsName(std::vector<std::string>& pluginsName)
        {
            for (const auto& plugin : s_plugins)
            {
                pluginsName.emplace_back(plugin->getPlugname());
            }

            return static_cast<int16_t>(pluginsName.size());
        }

    private:
        //加载插件
        static int16_t loadPlugins(const std::vector<std::string>& pluginsName,
            const std::string& folder = "")
        {
            for (const auto& pName : pluginsName)
            {
                ModuleAction::uptr modulePtr = std::make_unique<ModuleAction>();
                if (modulePtr && modulePtr->load(pName, folder))
                {
                    s_plugins.emplace_back(std::move(modulePtr));
                }
            }

            return s_plugins.size();
        }

        //卸载插件
        static void unloadPlugins()
        {
            while (!s_plugins.empty())
            {
                //类似栈上操作，后加载的先释放
                s_plugins.pop_back();
            }
        }
    };
}