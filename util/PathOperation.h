#pragma once

#include <string>
#include <string.h>

class PathOperation
{
public:
    //取到不含路径的文件名
    static std::string getFilenameWithoutPath(const std::string& originFilename)
    {
        const char* filename = originFilename.empty() ?
            strrchr(originFilename.c_str(), '/') : NULL;

        return (filename ? filename + 1 : originFilename);
    }
    //判断是否为不包含路径的文件名
    static bool filenameWithoutPath(const std::string& filename)
    {
        return (!filename.empty()) && (filename.find("/") == std::string::npos)
            && (filename.find("./") == std::string::npos);
    }
    //得到文件名的全路径
    static std::string getFullfilename(const std::string& folder,
        const std::string& filename)
    {
        if (folder.empty())
        {
            return filename;
        }

        std::string fullfilename = folder;
        if (folder[folder.size() - 1] != '/')
        {
            fullfilename += "/";
        }
        fullfilename += getFilenameWithoutPath(filename);
        return fullfilename;
    }
};