

#include <x3manager/source/CManager.h>
#include <x3framework/ModuleAction.h>

#include <algorithm>


using namespace x3plugin;

CManager::CManager()
{
}

CManager::~CManager()
{
}

//注册plugin到插件管理器
//creator:创建插件实例函数
//hmod:插件操作句柄
//clsids:插件中实例id
void CManager::registerPlugin(Creator creator, HMODULE hmod, const char** clsids)
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    auto iter = std::find(_plugins.begin(), _plugins.end(), Plugin(creator, hmod));
    if (iter == _plugins.end())
    {
        _plugins.push_back(Plugin(creator, hmod));

        for (; *clsids; ++clsids)
        {
            _clsmap.insert(CreatorPair(*clsids, creator));
        }
    }
}

//从插件管理器中删除插件
void CManager::unregisterPlugin(Creator creator)
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    for (std::vector<Plugin>::iterator it = _plugins.begin();
        it != _plugins.end(); ++it)
    {
        if (it->first == creator)
        {
            _plugins.erase(it);
            break;
        }
    }

    auto iter = _clsmap.begin();
    while (iter != _clsmap.end())
    {
        if (iter->second == creator)
        {
            _clsmap.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

bool CManager::createFromOthers(const std::string& clsid, int64_t iid, IObject** doublePointToObj)
{
    Creator creator = findPluginByClassID(clsid);
    if (creator && creator(clsid.c_str(), iid, doublePointToObj))
    {
        return true;
    }

    return false;
}

HMODULE CManager::findModuleByFileName(const std::string& filename)
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    for (auto& plugin : _plugins)
    {
        std::string findfilename = "";
        ModuleAction::getModulefilename(plugin.second, findfilename);

        if (filename == findfilename)
        {
            return plugin.second;
        }
    }

    return NULL;
}

int32_t CManager::getPluginCount() const
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    return 1 + _plugins.size();
}

void CManager::getPluginFiles(std::vector<std::string>& files) const
{
    files.clear();
    std::string filename = "";
    ModuleAction::getModulefilename(getModuleHandle(), filename);
    files.push_back(filename);

    std::lock_guard<std::mutex> locker(_registerMutex);
    for (auto& plugin : _plugins)
    {
        ModuleAction::getModulefilename(plugin.second, filename);
        files.push_back(filename);
    }
}

Creator CManager::findPluginByClassID(const std::string& clsid) const
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    const auto& findIter = _clsmap.find(clsid);
    return (findIter != _clsmap.end()) ? findIter->second : NULL;
}