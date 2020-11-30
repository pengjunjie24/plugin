

#include <x3manager/source/CManager.h>
#include <x3manager/interface/Iplugins.h>

#include <util/Noncopyable.h>
#include <x3framework/PluginImpl.h>
#include <x3framework/Objptr.hpp>
#include <x3framework/ModuleMacro.h>

namespace x3plugin
{
    XBEGIN_DEFINE_MODULE()
        XDEFINE_CLASSMAP_ENTRY_SINGLETON(CManager)
    XEND_DEFINE_MODULE()

    OUTAPI bool x3InitializePlugin()
    {
        return true;
    }

    OUTAPI void x3UninitializePlugin()
    {
    }

    OUTAPI bool x3RegisterPlugin(Creator creator,
        HMODULE hmod, const char** clsids)
    {
        Object<IRegister> reg(clsidX3manager);

        if (reg)
        {
            reg->registerPlugin(creator, hmod, clsids);
        }

        return true;
    }

    OUTAPI bool x3UnregisterPlugin(Creator creator)
    {
        Object<IRegister> reg(clsidX3manager);
        if (reg && creator)
        {
            reg->unregisterPlugin(creator);
        }

        return true;
    }

    OUTAPI HMODULE x3FindModule(const char* filename)
    {
        Object<IRegister> reg(clsidX3manager);
        if (reg)
        {
            return reg->findModuleByFileName(filename);
        }

        return NULL;
    }

    OUTAPI bool x3CreateObject(const char* clsid, long iid, IObject** p)
    {
        if (x3InternalCreate(clsid, iid, p))
        {
            return true;
        }

        Object<IRegister> reg(clsidX3manager);
        return reg && reg->createFromOthers(clsid, iid, p);
    }
}