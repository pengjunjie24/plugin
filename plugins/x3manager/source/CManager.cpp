

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
void CManager::registerPlugin(Creator creator, Destructor destructor, HMODULE hmod, const char* clsid)
{
    std::lock_guard<std::mutex> locker(_registerMutex);

    auto iter = _clsPluginmap.find(clsid);
    if (iter == _clsPluginmap.end())
    {
        _clsPluginmap.insert(std::make_pair(clsid, std::make_tuple(creator, destructor, ModuleAction::getModulefilename(hmod))));
    }
}

//从插件管理器中删除插件
void CManager::unregisterPlugin(const char* clsid)
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    const auto& iter = _clsPluginmap.find(clsid);
    if (iter != _clsPluginmap.end())
    {
        Destructor destructor = std::get<DESTRUCTOR_FUNC>(iter->second);
        if (destructor)
        {
            destructor(iter->first.c_str());
        }
        _clsPluginmap.erase(iter);
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

bool CManager::findModuleByFileName(const std::string& filename)
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    for (auto& clsplugin : _clsPluginmap)
    {
        if (filename == std::get<FILE_NAME>(clsplugin.second))
        {
            return true;
        }
    }

    return false;
}

int32_t CManager::getPluginCount() const
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    return 1 + _clsPluginmap.size();
}

void CManager::getPluginFiles(std::vector<std::string>& files) const
{
    files.clear();
    std::string filename = ModuleAction::getModulefilename(getModuleHandle());
    files.push_back(filename);

    std::lock_guard<std::mutex> locker(_registerMutex);
    for (auto& clsPlugin : _clsPluginmap)
    {
        const auto& plugin = clsPlugin.second;
        files.push_back(std::get<FILE_NAME>(plugin));
    }
}

Creator CManager::findPluginByClassID(const std::string& clsid) const
{
    std::lock_guard<std::mutex> locker(_registerMutex);
    const auto& findIter = _clsPluginmap.find(clsid);
    return (findIter != _clsPluginmap.end()) ? std::get<CREATOR_FUNC>(findIter->second) : NULL;
}