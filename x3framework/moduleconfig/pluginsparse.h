
#pragma once

#include <x3framework/moduleconfig/moduleconfigure.h>
#include <util/PathOperation.h>

#include <fstream>
#include <list>

//解析插件的配置文件，按照配置文件确定初始化顺序
class PluginsParse
{
public:
    //pluginfolder: 插件配置文件所在目录
    PluginsParse()
        : _parse(false)
    {
    }

    //在parsePluginConf函数调用后就不能再调用了
    void setExcludePlugins(const std::vector<std::string>& excludePluginVec)
    {
        if (!_parse)
        {
            _excludePlugins = excludePluginVec;
        }
    }

    //解析插件配置文件，按初始化顺序放入就绪队列，只能调用一次
    bool parsePluginConf(const std::string& pluginfolder)
    {
        if (!_parse)
        {
            _parse = true;
            std::vector<std::string> dirs;
            PathOperation::getFileOnCurPath(pluginfolder, PathOperation::DIR_FILE, dirs);

            getModuleConfigVec(pluginfolder, dirs);
            parseExcludePlugins();
            return getReadyPlugins();
        }
        fprintf(stderr, "parsePluginConf has been called\n");
        return false;
    }

    //获取插件加载信息
    std::string getPluginConfigInfo()
    {
        if (_parse)
        {
            std::string includePlugin = "include plugin:";
            for (const auto& plugin : _plugins)
            {
                includePlugin += " ";
                includePlugin += plugin;
            }
            includePlugin += "\n";

            std::string excludePlugin = "exclude plugin:";
            for (const auto& plugin : _excludePlugins)
            {
                excludePlugin += " ";
                excludePlugin += plugin;
            }
            excludePlugin += "\n";
            return (includePlugin + excludePlugin);
        }
        return "not call parsePluginConf function, don't get detail information\n";
    }

    const std::vector<std::string>& getReadyPluginVec() { return _plugins; }
    const std::vector<std::string>& getNotLoadPluginVec() { return _excludePlugins; }
    const std::vector<ModuleConifgure>& getConfigureVec() { return _pluginconfigVec; }

private:
    //获取所有插件的配置文件
    //pluginfolder: 所有插件的配置文件目录
    //plugindirVec: 每个插件配置文件的目录
    void getModuleConfigVec(const std::string pluginfolder, const std::vector<std::string>& plugindirVec)
    {
        std::string fullfilename = "";
        for (const auto& dir : plugindirVec)
        {
            fullfilename = "";
            fullfilename = PathOperation::getFullfilename(PathOperation::getFullfilename(pluginfolder, dir), s_configFile);
            if (PathOperation::fileExist(PathOperation::REGULAR_FILE, fullfilename))
            {
                std::ifstream jsonfile(fullfilename.c_str());
                if (!jsonfile.is_open())
                {
                    continue;
                }
                nlohmann::json pluginconfigjson;
                jsonfile >> pluginconfigjson;
                _pluginconfigVec.emplace_back(std::move(pluginconfigjson));
            }
        }
    }

    //解析被排除的插件
    void parseExcludePlugins()
    {
        if (_excludePlugins.empty())
        {
            return;
        }

        size_t excludePluginSize = _excludePlugins.size();
        do {
            excludePluginSize = _excludePlugins.size();
            for (const ModuleConifgure& configure : _pluginconfigVec)
            {
                for (const std::string& dependencyPluginName : configure.dependenciesPluginsName)
                {
                    //当前插件不在需要排除加载的插件中，但是所依赖的插件却在排除加载的插件中
                    //需要将当前插件也排除加载
                    if ((std::find(_excludePlugins.begin(), _excludePlugins.end(),
                        configure.pluginName) == _excludePlugins.end()) &&
                        (std::find(_excludePlugins.begin(), _excludePlugins.end(),
                        dependencyPluginName) != _excludePlugins.end()))
                    {
                        _excludePlugins.push_back(configure.pluginName);
                    }
                }
            }
        } while (excludePluginSize != _excludePlugins.size());
    }

    //按照配置文件，解析插件加载顺序
    bool getReadyPlugins()
    {
        std::list<ModuleConifgure> configList;//使用list容器，便于删除
        std::copy(_pluginconfigVec.begin(), _pluginconfigVec.end(),
            std::back_inserter(configList));

        //将不需要加载的插件排除
        if (!_excludePlugins.empty())
        {
            for (auto confIter = configList.begin(); confIter != configList.end();)
            {
                if (std::find(_excludePlugins.begin(), _excludePlugins.end(),
                    confIter->pluginName) != _excludePlugins.end())
                {
                    configList.erase(confIter++);
                }
                else
                {
                    confIter++;
                }
            }
        }

        size_t prePluginsize = _plugins.size();
        while (!configList.empty())
        {
            for (auto confIter = configList.begin(); confIter != configList.end();)
            {
                //当前插件模块不依赖其他模块或者依赖的模块已经进入就绪队列
                //就将当前模块加入就绪队列
                if (confIter->dependenciesPluginsName.empty() ||
                    isDependenPluginLoaded(confIter->dependenciesPluginsName))
                {
                    _plugins.emplace_back(std::move(confIter->pluginName));
                    configList.erase(confIter++);
                }
                else
                {
                    confIter++;
                }
            }

            //经过一次循环后加载插件数量没变，说明出现插件循环依赖
            if (prePluginsize >= _plugins.size())
            {
                std::string errPluginStr = "";
                for (const auto& errPlugin : configList)
                {
                    errPluginStr += errPlugin.pluginName;
                    errPluginStr += " ";
                }

                //TODO: 需要输出循环链, 便于后续排查问题
                fprintf(stderr, "These %s plugins has a circular dependency\n", errPluginStr.c_str());
                assert(false);
                return false;
            }

            prePluginsize = _plugins.size();//更新已加载插件数目
        }

        printf("%s\n", getPluginConfigInfo().c_str());
        return true;
    }

    //dependenPlugins容器中插件是否已经加入就绪队列
    bool isDependenPluginLoaded(const std::vector<std::string>& dependenPlugins)
    {
        for (const auto& pluginName : dependenPlugins)
        {
            if (std::find(_plugins.begin(), _plugins.end(), pluginName) == _plugins.end())
            {
                return false;
            }
        }

        return true;
    }

    bool _parse;//配置文件是否解析完成
    std::vector<ModuleConifgure> _pluginconfigVec;//插件配置文件
    std::vector<std::string> _plugins;//就绪的插件队列，按照初始化顺序进行排列
    std::vector<std::string> _excludePlugins;//排除的插件，不进行加载

    static constexpr const char* s_configFile = "module.json";
};