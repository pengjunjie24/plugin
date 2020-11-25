
#pragma once
#include <x3framework/Iobject.h>

#include <vector>
#include <string>

namespace x3plugin
{
    const char* const clsidX3manager = "1137fdae-fd69-4d4c-b3d1-e8f57b55f27e";

    class IPlugins : public IObject
    {
        X3DEFINE_IID(IPlugins);

        virtual int32_t getPluginCount() const = 0;//获取插件数量
        virtual void getPluginFiles(std::vector<std::string>& files) const = 0;//获取插件文件名
    };
}