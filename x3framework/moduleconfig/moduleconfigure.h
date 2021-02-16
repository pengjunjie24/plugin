
#pragma once

#include <thirdparty/nlohmannjson-3.9.1/include/json.hpp>

struct ModuleConifgure
{
    std::string pluginName;//插件名称
    std::vector<std::string> dependenciesPluginsName;//当前插件依赖的插件
};

void from_json(const nlohmann::json& modulejson, ModuleConifgure& configure)
{
    modulejson.at("name").get_to(configure.pluginName);
    for (size_t i = 0; i < modulejson["dependencies"].size(); ++i)
    {
        std::string dependPlugin;
        modulejson["dependencies"][i].get_to(dependPlugin);
        configure.dependenciesPluginsName.emplace_back(std::move(dependPlugin));
    }
}