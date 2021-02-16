#pragma once

#include <string>
#include <vector>
#include <string.h>
#include <dirent.h>

class PathOperation
{
public:
    //判断传入文件名是否正确
    static bool isRegularFile(const std::string& filename)
    {
        if (!filename.empty())
        {
            if (*filename.rbegin() != '/')
            {
                return true;
            }
        }

        return false;
    }

    //取到不含路径的文件名
    static std::string getFilenameWithoutPath(const std::string& originFilename)
    {
        if (isRegularFile(originFilename))
        {
            std::size_t findplace = originFilename.find_last_of('/');
            return (findplace != std::string::npos) ?
                std::string{ originFilename, findplace + 1 } : originFilename;
        }
        return "";
    }
    //判断是否为不包含路径的文件名
    static bool filenameWithoutPath(const std::string& filename)
    {
        return (isRegularFile(filename) && (filename.find("/") == std::string::npos)
            && (filename.find("./") == std::string::npos));
    }
    //得到文件名的全路径
    static std::string getFullfilename(const std::string& folder,
        const std::string& filename)
    {
        if (folder.empty() || !isRegularFile(filename))
        {
            return "";
        }

        std::string fullfilename = folder;
        if (folder[folder.size() - 1] != '/')
        {
            fullfilename += "/";
        }
        fullfilename += getFilenameWithoutPath(filename);
        return fullfilename;
    }

    //获取全路径文件名的路径
    static std::string getFolder(const std::string& fullfilename)
    {
        std::string fullFolder = fullfilename;
        std::size_t found = fullFolder.rfind('/');
        if (found != std::string::npos)
        {
            return fullFolder.erase(found, fullFolder.length() - found);
        }
        return fullFolder;
    }

    enum FILE_TYPE
    {
        BLOCK_FILE = DT_BLK,        //块设备
        CHAR_FILE = DT_CHR,         //字符设备
        DIR_FILE = DT_DIR,          //目录
        LINK_FILE = DT_LNK,         //软连接
        FIFO_FILE = DT_FIFO,        //管道
        REGULAR_FILE = DT_REG,      //普通文件
        SOCKET_FILE = DT_SOCK,      //套接字文件
        UNKOWN_FILE = DT_UNKNOWN,   //未知
    };
    //获取当前路径下文件
    static bool getFileOnCurPath(const std::string& curPath, const FILE_TYPE& filetype, std::vector<std::string>& files)
    {
        DIR* pCurPath = opendir(curPath.c_str());
        if (pCurPath == NULL)
        {
            fprintf(stderr, "the path %s is error\n", curPath.c_str());
            return false;
        }

        struct dirent* pdirent = NULL;
        while ((pdirent = readdir(pCurPath)) != NULL)
        {
            if ((strcmp(".", pdirent->d_name) == 0 || strcmp("..", pdirent->d_name) == 0))
            {
                continue;
            }
            if (pdirent->d_type == filetype)
            {
                files.push_back(pdirent->d_name);
            }
        }

        closedir(pCurPath);
        return true;
    }

    //判断文件是否存在
    //filetype: 判断的文件的类型
    //filename: 文件的全路径
    static bool fileExist(const FILE_TYPE& filetype, const std::string& fullfilename)
    {
        std::string folder = getFolder(fullfilename);//获取路径
        std::string filename = getFilenameWithoutPath(fullfilename);//获取文件名

        std::vector<std::string> filenameVec;
        getFileOnCurPath(folder, filetype, filenameVec);//获取当前目录下的文件
        for (const auto& existFile : filenameVec)
        {
            if (existFile == filename)//判断文件是否在当前目录下
            {
                return true;
            }
        }

        return false;
    }
};