
#pragma once

#include <util/Singleton.hpp>
#include <x3framework/moduleconfig/pluginsparse.h>
#include <x3framework/modulebehavior/ModuleAction.h>
#include <x3framework/modulebehavior/Useplugins.h>

namespace x3plugin
{
    //用户调用
    //整合了用户配置管理及插件实际加载
    class ModuleManager: public Singleton<ModuleManager>
    {
    public:
        ~ModuleManager()
        {
            unloadPlugins();
        }

        bool loadPlugin(const std::string configFolder, const std::string pluginFolder)
        {
            bool ret = false;
            if (_pluginParse.parsePluginConf(configFolder))
            {
                const auto& parsePlugins = _pluginParse.getReadyPluginVec();
                loadPlugins(parsePlugins, pluginFolder);//加载插件
                std::vector<std::string> realityPlugins;
                getPluginsName(realityPlugins);
                if (realityPlugins.size() != parsePlugins.size())
                {
                    fprintf(stderr, "load plugin number error, should load number %lu, reality load number %lu\n",
                        parsePlugins.size(), realityPlugins.size());
                }
                ret = true;
            }
            return ret;
        }

        const std::vector<ModuleConifgure>& getPluginsConfigure() { return _pluginParse.getConfigureVec(); }
        void getLoadPluginsName(std::vector<std::string>& pluginsName) { return getPluginsName(pluginsName); }

    private:
        friend Singleton<ModuleManager>;
        ModuleManager() = default;

        PluginsParse _pluginParse;
    };

    //获取插件接口
    template<typename T>
    static void getPlugins(const std::string& clsid, AnyObject& dstObj)
    {
        //通过clsid获取plugin名称的仿函数
        auto getPluginNameByClsid = [](const std::string& clsid)->std::string {
            std::string targetPluginName = "";
            for (const auto& plugin : s_plugins)
            {
                if (plugin->getClsid() == clsid)
                {
                    targetPluginName = plugin->getPlugname();
                    break;
                }
                return targetPluginName;
            }
        };

        std::string srcPluginName = getPluginNameByClsid(T::_getClassId());
        std::string dstPluginName = getPluginNameByClsid(clsid);
        assert(!srcPluginName.empty() && !dstPluginName.empty());

        if (!srcPluginName.empty() && !dstPluginName.empty())
        {
            const std::vector<ModuleConifgure>& moduleConfigs =
                ModuleManager::instance()->getPluginsConfigure();
            ModuleConifgure srcConfig;
            for (const auto& config : moduleConfigs)
            {
                if (config.pluginName == srcPluginName)
                {
                    srcConfig = config;
                    break;
                }
            }

            for (const auto &dependPluginName : srcConfig.dependenciesPluginsName)
            {
                if (dependPluginName == dstPluginName)
                {
                    dstObj = AnyObject(clsid.c_str());
                }
            }
        }
        else
        {
            fprintf(stderr, "srcPluginName: %s, dstPluginName: %s\n",
                srcPluginName.c_str(), dstPluginName.c_str());
        }
    }
}