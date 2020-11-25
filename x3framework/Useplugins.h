
#pragma once

#include <x3framework/ModuleAction.h>

#include <vector>

namespace x3plugin
{
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
            s_plugins.clear();
        }

        static std::vector<ModuleAction::uptr> s_plugins;
    };

    std::vector<ModuleAction::uptr> AutoLoadPlugins::s_plugins;
}