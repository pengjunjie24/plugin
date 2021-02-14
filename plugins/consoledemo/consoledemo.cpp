

#include <calculation/interface/Calculate1.h>
#include <calculation/interface/Calculate2.h>
#include <calculation/interface/Creator.h>
#include <printer/interface/PrintInterface.h>

#include <x3framework/Objptr.hpp>
#include <x3framework/Useplugins.h>

#include "../x3config.h"

#include <string>
#include <vector>


void test()
{
    std::vector<std::string> pluginsName;
    x3plugin::AutoLoadPlugins::getPluginsName(pluginsName);
    printf("---------------------plugin number: %d--------------------\n", (int32_t)pluginsName.size());
    for (const auto& plugin : pluginsName)
    {
        printf("%s\n", plugin.c_str());
    }

    printf("----------------------------------------------------------\n");

    x3plugin::Object<Creation> plsCls3(c_calculationClsid);
    if (plsCls3)
    {
        printf("The plugin is loaded (%s in %s).\n",
            plsCls3->getInterfaceName(), plsCls3->getClassName());

        x3plugin::Object<Calculate1> plsCls = plsCls3->createSimple();
        if (plsCls)
        {
            int sum = plsCls->add(1, 2);
            printf("plsCls->add(1, 2): %d\n", sum);
            sum = plsCls->add(4, 9);
            printf("plsCls->add(4, 9): %d\n", sum);
        }

        x3plugin::Object<Calculate1> plsCls1(c_calculationClsid);
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

    x3plugin::Object<PrintInterface> pltTemp(c_printClsid);
    if (pltTemp)
    {
        int sum = pltTemp->add(1, 7);
        printf("PrintInterface pltTemp->add(1, 7): %d\n", sum);
    }
    else
    {
        printf("x3plugin::Object<PrintInterface> pltTemp load failed\n");
    }
}


int main()
{
    printf("PLUGINS_PATH: %s\n", PLUGINS_PATH);

    std::vector<std::string> pluginVec = {"calculation.pln", "printer.pln"};

     x3plugin::AutoLoadPlugins autoload(pluginVec, PLUGINS_PATH);
    test();
}