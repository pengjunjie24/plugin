
#include <x3framework/moduleconfig/pluginsparse.h>

#include "config.h"

int main()
{
    PluginsParse pluginsparseDemo(CMAKE_CURRENT_SOURCE_DIR);
    pluginsparseDemo.setExcludePlugins({ "plugin3" });
    pluginsparseDemo.parsePluginConf();
    std::vector<std::string> pluginVec = pluginsparseDemo.getReadyPluginVec();

    printf("------------plugin loading sequence------------\n");
    for (const auto& plugin : pluginVec)
    {
        printf("%s\n", plugin.c_str());
    }
    printf("-----------------------------------------------\n");

    return 0;
}