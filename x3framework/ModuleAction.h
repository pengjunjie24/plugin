
#pragma once

#include <x3framework/PluginDef.h>
#include <util/Noncopyable.h>
#include <util/PathOperation.h>

#include <dlfcn.h>
#include <assert.h>
#include <memory>

namespace x3plugin
{
    //插件加载器
    class ModuleAction: Noncopyable
    {
    public:
        typedef std::unique_ptr<ModuleAction> uptr;

        ModuleAction()
            : _hmod(NULL)
            , _plugname("")
            , _plugfolder("")
            , _plugclsid("")
        {
        }

        ~ModuleAction()
        {
            unload();
        }

        //加载插件
        //filename: 插件名字
        //folder: 插件路径
        bool load(const std::string& filename, const std::string& folder = "")
        {
            //从插件管理器中查找插件
            assert(!_hmod);//确保该模块未加载
            _plugname = filename;
            _plugfolder = folder;
           _hmod = loadLibrary(_plugfolder, _plugname, _plugclsid);
            return _hmod != NULL;
        }

        //卸载插件
        void unload()
        {
            if (_hmod)
            {
                freeLibrary(_hmod);
                _hmod = NULL;
            }
        }

        const std::string& getPlugname() { return _plugname; }
        const std::string& getClsid() { return _plugclsid; }
        //通过函数名得到函数得到函数指针
        PROC getFunc(const std::string& funcName) const { return _hmod ? getProcAddress(_hmod, funcName) : NULL; }

    private:
        static HMODULE loadLibrary(const std::string&folder, const std::string& filename, std::string& clsidStr)
        {
            if (filename.empty())
            {
                fprintf(stderr, "ModuleAction::loadLibrary filename empty\n");
                return NULL;
            }

            std::string fullname = PathOperation::getFullfilename(folder, filename);
            HMODULE hmod = dlopen(fullname.c_str(), RTLD_LAZY);
            if (hmod)
            {
                if (!ondlopen(hmod, clsidStr))
                {
                    fprintf(stderr, "%s ondlopen failed\n", filename.c_str());
                    freeLibrary(hmod);
                    assert(false);
                    return NULL;
                }
            }
            return hmod;
        }

        static bool freeLibrary(HMODULE hlib)
        {
            typedef void(*FreePluginFunc)();
            FreePluginFunc freeFunc = (FreePluginFunc)getProcAddress(hlib, "x3FreePlugin");
            if (freeFunc)
            {
                freeFunc();
            }

            int ret = hlib ? dlclose(hlib) : 0;
            if (ret)
            {
                fprintf(stderr, dlerror());
            }

            return (ret == 0);
        }

        static PROC getProcAddress(HMODULE hmod, const std::string& name)
        {
            void* sym = NULL;

            if (hmod)
            {
                sym = dlsym(hmod, name.c_str());
                fprintf(stderr, dlerror());
            }

            return sym;
        }

        //插件加载成功后需调用的初始化函数
        static bool ondlopen(HMODULE hmod, std::string& plugclsid)
        {
            const char* clsidtmp = NULL;
            typedef bool(*InitFunc)(HMODULE, const char*&);
            InitFunc initfunc = (InitFunc)getProcAddress(hmod, "x3InitPlugin");

            if (initfunc)
            {
                initfunc(hmod, clsidtmp);
                plugclsid = clsidtmp;
                return true;
            }

            return false;
        }

    private:
        HMODULE _hmod;//插件操作句柄
        std::string _plugname;//插件名称
        std::string _plugfolder;//插件路徑
        std::string _plugclsid;//插件的guid
    };


}
