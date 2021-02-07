

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

    OUTAPI bool x3RegisterPlugin(Creator creator, Destructor destructor,
        HMODULE hmod, const char* clsid)
    {
        Object<IRegister> reg(clsidX3manager);

        if (reg)
        {
            reg->registerPlugin(creator, destructor, hmod, clsid);
        }

        return true;
    }

    OUTAPI bool x3UnregisterPlugin(const char* clsid)
    {
        Object<IRegister> reg(clsidX3manager);
        if (reg && (*clsid != 0))
        {
            reg->unregisterPlugin(clsid);
        }

        return true;
    }

    OUTAPI bool x3FindModule(const char* filename)
    {
        Object<IRegister> reg(clsidX3manager);
        if (reg)
        {
            return reg->findModuleByFileName(filename);
        }

        return NULL;
    }

    OUTAPI bool x3CreateObject(const char* clsid, int64_t iid, IObject** p)
    {
        if (x3InternalCreate(clsid, iid, p))
        {
            return true;
        }

        Object<IRegister> reg(clsidX3manager);
        return reg && reg->createFromOthers(clsid, iid, p);
    }
}