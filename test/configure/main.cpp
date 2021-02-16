
#include <x3framework/moduleconfig/pluginsparse.h>

#include "config.h"

int main()
{
    PluginsParse pluginsparseDemo(CMAKE_CURRENT_SOURCE_DIR);
    std::vector<std::string> pluginVec = pluginsparseDemo.getReadyPluginVec();

    for (const auto& plugin : pluginVec)
    {
        printf("------------plugin loading sequence------------\n");
        printf("%s\n", plugin.c_str());
        printf("-----------------------------------------------\n");
    }

    return 0;
}