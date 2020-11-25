
#include <plsimple/interface/isimple.h>
#include <x3framework/Objptr.hpp>
#include <x3framework/Useplugins.h>
#include <x3config.h>

#include <string>
#include <vector>

void test()
{
    x3plugin::Object<ISimple> plsCls(plsClsidSimple);
    if (plsCls)
    {
        printf("The plugin is loaded (%s in %s).\n",
            plsCls->getInterfaceName(), plsCls->getClassName());

        int sum = plsCls->add(1, 2);
        printf("plsCls->add(1, 2): %d\n", sum);
    }
    else
    {
        printf("The plugin is not loaded.\n");
    }
}

int main()
{
    printf("PLUGINS_PATH: %s\n", PLUGINS_PATH);

    std::vector<std::string> pluginVec = {"x3manager.pln",
        "plsimple.pln", "pltempl.pln"};

    x3plugin::AutoLoadPlugins autoload(pluginVec);
    test();
}