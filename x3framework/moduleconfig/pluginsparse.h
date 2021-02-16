
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
    PluginsParse(const std::string& pluginfolder)
    {
        std::vector<std::string> dirs;
        PathOperation::getFileOnCurPath(pluginfolder, PathOperation::DIR_FILE, dirs);

        std::vector<ModuleConifgure> configVec;
        getModuleConfigVec(pluginfolder, dirs, configVec);

        parsePluginConf(configVec);
    }

    const std::vector<std::string>& getReadyPluginVec() { return _plugins; }

private:
    //获取所有插件的配置文件
    //pluginfolder: 所有插件的配置文件目录
    //plugindirVec: 每个插件配置文件的目录
    //configVec: 获取每个插件的配置
    static void getModuleConfigVec(const std::string pluginfolder, const std::vector<std::string>& plugindirVec, std::vector<ModuleConifgure>& configVec)
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
                configVec.emplace_back(std::move(pluginconfigjson));
            }
        }
    }

    //按照配置文件，解析插件加载顺序
    bool parsePluginConf(const std::vector<ModuleConifgure>& configVec)
    {
        std::list<ModuleConifgure> configList;//使用list容器，便于删除
        std::copy(configVec.begin(), configVec.end(),
            std::back_inserter(configList));

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

    std::vector<std::string> _plugins;//就绪的插件队列，按照初始化顺序进行排列

    static constexpr const char* s_configFile = "module.json";
};