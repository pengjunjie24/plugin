
#include <x3framework/moduleconfig/pluginsparse.h>

#include "config.h"

int main()
{
    PluginsParse pluginsparseDemo;
    pluginsparseDemo.setExcludePlugins({ "plugin3" });
    pluginsparseDemo.parsePluginConf(CMAKE_CURRENT_SOURCE_DIR);
    std::vector<std::string> pluginVec = pluginsparseDemo.getReadyPluginVec();

    printf("------------plugin loading sequence------------\n");
    for (const auto& plugin : pluginVec)
    {
        printf("%s\n", plugin.c_str());
    }
    printf("-----------------------------------------------\n");

    return 0;
}