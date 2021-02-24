

#include <calculation/interface/Calculate1.h>
#include <calculation/interface/Calculate2.h>
#include <calculation/interface/Creator.h>
#include <printer/interface/PrintInterface.h>

#include <x3framework/moduleclass/Objptr.hpp>
#include <x3framework/ModuleManager.h>

#include "../x3config.h"

#include <string>
#include <vector>


void test()
{
    std::vector<std::string> pluginsName;
    x3plugin::ModuleManager::instance()->getLoadPluginsName(pluginsName);
    printf("---------------------plugin number: %d--------------------\n", (int32_t)pluginsName.size());
    for (const auto& plugin : pluginsName)
    {
        printf("%s\n", plugin.c_str());
    }

    printf("----------------------------------------------------------\n");

#if 0
    x3plugin::Object<Creation> plsCls3;
    x3plugin::getPlugins<PrintImpl>(c_calculationClsid, plsCls3);
    if (plsCls3)
    {
        printf("The plugin is loaded (%s in %s).\n",
            plsCls3->getInterfaceName(), plsCls3->getClassName());

        x3plugin::Object<Calculate1> plsCls1;
        x3plugin::getPlugins<PrintImpl>(c_calculationClsid, plsCls3);
        if (plsCls1)
        {
            int sum = plsCls1->add(2, 3);
            printf("plsCls->add(2, 3): %d\n", sum);
            sum = plsCls1->add(4, 6);
            printf("plsCls->add(4, 6): %d\n", sum);
        }
    }
    else
    {
        printf("The x3plugin::Object<Calculate1> plugin is not loaded.\n");
    }
#endif
}


int main()
{
    printf("PLUGINS_PATH: %s\n", PLUGINS_PATH);
    printf("CMAKE_CURRENT_SOURCE_DIR: %s\n", CMAKE_CURRENT_SOURCE_DIR);

    x3plugin::ModuleManager::instance()->loadPlugin(CMAKE_CURRENT_SOURCE_DIR, PLUGINS_PATH);
    test();
}