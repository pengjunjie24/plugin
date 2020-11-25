
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

        ModuleAction(const std::string& filename = "")
            : _hmod(NULL)
            , _loadnew(false)
        {
            if (!filename.empty())
            {
                load(filename);
            }
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
            assert(!_hmod);//调用load时确保插件未加载
            std::string fullfilename = PathOperation::getFullfilename(folder, filename);
            _hmod = findHandleModule(filename);
            _loadnew = !_hmod;//若在插件管理器中找到代表插件已经被加载

            //当前插件未加载
            if (!_hmod)
            {
                _hmod = loadLibrary(filename, folder);
            }

            return _hmod != NULL;
        }

        //卸载插件
        void unload()
        {
            if (_hmod)
            {
                //如果插件被加载过，要先卸载
                if (_loadnew)
                {
                    freeLibrary(_hmod);
                }
                _hmod = NULL;
            }
        }

        HMODULE getModule() const { return _hmod; }

        //通过函数名得到函数得到函数指针
        PROC getFunc(std::string& funcName) const { return _hmod ? getProcAddress(_hmod, funcName) : NULL; }

        static HMODULE getManageMod() { return s_manageMod; }

        static HMODULE loadLibrary(const std::string& filename, const std::string& folder)
        {
            if (filename.empty())
            {
                fprintf(stderr, "ModuleAction::loadLibrary filename empty\n");
                return NULL;
            }

            //s_manageMod插件加载后才能加载其他插件
            if (!s_manageMod)
            {
                if (!loadManageMod(folder))
                {
                    return NULL;
                }
            }

            if (filename == s_manageName)
            {
                return NULL;
            }

            std::string fullfilename = PathOperation::getFullfilename(folder, filename);
            HMODULE hmod = dlopen(fullfilename.c_str(), RTLD_LAZY);
            if (hmod)
            {
                if (!ondlopen(hmod, fullfilename.c_str()))
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

        //通过filename得到handle
        static HMODULE findHandleModule(const std::string& filename)
        {
            if (filename.empty())
            {
                return NULL;
            }

            if (filename == s_manageName)
            {
                return s_manageMod;
            }

            //通过OutputFindModule函数找到module
            if (s_manageMod)
            {
                typedef HMODULE(*FindModule)(const char*);
                FindModule findModule = (FindModule)getProcAddress(s_manageMod, "x3FindModule");

                return findModule ? findModule(filename.c_str()) : NULL;
            }

            return NULL;
        }

        //插件加载成功后需调用的初始化函数
        static bool ondlopen(HMODULE hmod, const char* fullfilename)
        {
            typedef bool(*InitFunc)(HMODULE, HMODULE, const char*);
            InitFunc initfunc = (InitFunc)getProcAddress(hmod, "x3InitPlugin");

            return !initfunc || initfunc(hmod, s_manageMod, fullfilename);
        }

        static void getModulefilename(HMODULE hmod, std::string& fullfilename)
        {
            char filename[s_filenameSize] = { 0 };
            typedef bool(*FindFilenameFunc)(char*, int16_t);
            FindFilenameFunc filenameFunc = (FindFilenameFunc)getProcAddress(hmod, "getdlname");
            if (filenameFunc)
            {
                filenameFunc(filename, s_filenameSize);
                fullfilename = filename;
            }
        }

        //加载插件管理器
        static bool loadManageMod(const std::string& folder)
        {
            assert(!s_manageMod);//开始没有加载s_manageMod

            std::string fullmanageflie =
                PathOperation::getFullfilename(folder, s_manageName);
            s_manageMod = dlopen(fullmanageflie.c_str(), RTLD_LAZY);

            assert(s_manageMod);
            if (!s_manageMod)
            {
                fprintf(stderr, "s_manageMod %s load failed\n", fullmanageflie.c_str());
            }

            return s_manageMod ? true : false;
        }

    private:
        HMODULE _hmod;//插件操作句柄
        bool _loadnew;

        static HMODULE s_manageMod;//插件管理器
        static const std::string s_manageName;
        static const int16_t s_filenameSize = 256;
    };


}
