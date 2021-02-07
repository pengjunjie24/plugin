
#include <plsimple/interface/isimple.h>
#include <plsimple/interface/isimple2.h>
#include <plsimple/interface/isimple3.h>
#include <pltempl/interface/pltisimple.h>
#include <x3framework/Objptr.hpp>
#include <x3framework/Useplugins.h>
#include "../x3config.h"

#include <string>
#include <vector>

void test()
{
    x3plugin::Object<ISimple3> plsCls3(plsClsidSimple);
    if (plsCls3)
    {
        printf("The plugin is loaded (%s in %s).\n",
            plsCls3->getInterfaceName(), plsCls3->getClassName());

        x3plugin::Object<ISimple> plsCls = plsCls3->createSimple();
        if (plsCls)
        {
            int sum = plsCls->add(1, 2);
            printf("plsCls->add(1, 2): %d\n", sum);
            sum = plsCls->add(4, 9);
            printf("plsCls->add(4, 9): %d\n", sum);
        }

        x3plugin::Object<ISimple> plsCls1(plsClsidSimple);
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
        printf("The x3plugin::Object<ISimple3> plugin is not loaded.\n");
    }

    x3plugin::Object<ISimple> pltTemp(IsClsidSimple);
    if (pltTemp)
    {
        int sum = pltTemp->add(1, 7);
        printf("IsClsidSimple pltTemp->add(1, 7): %d\n", sum);
    }
}

int main()
{
    printf("PLUGINS_PATH: %s\n", PLUGINS_PATH);

    std::vector<std::string> pluginVec = {"x3manager.pln",
        "plsimple.pln", "pltempl.pln"};

    x3plugin::AutoLoadPlugins autoload(pluginVec, PLUGINS_PATH);
    test();
}