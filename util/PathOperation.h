#pragma once

#include <string>
#include <vector>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

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
        UNKOWN_FILE = 0,    //未知
        REGULAR_FILE,       //普通文件
        DIR_FILE,           //目录
        CHAR_FILE,          //字符设备
        BLOCK_FILE,         //块设备
        LINK_FILE,          //软连接
        SOCKET_FILE,        //套接字文件
        FIFO_FILE,          //管道
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

        std::string fullname = "";
        struct dirent* pdirent = NULL;
        struct stat filebuf;
        while ((pdirent = readdir(pCurPath)) != NULL)
        {
            if ((strcmp(".", pdirent->d_name) == 0 || strcmp("..", pdirent->d_name) == 0))
            {
                continue;
            }

            fullname = getFullfilename(curPath, pdirent->d_name);
            if (stat(fullname.c_str(), &filebuf) != 0)
            {
                fprintf(stderr, "%s get file info failed, errno %d\n", fullname.c_str(), errno);
                return false;
            }
            if ((S_ISREG(filebuf.st_mode) && filetype == REGULAR_FILE) ||
                (S_ISDIR(filebuf.st_mode) && filetype == DIR_FILE) ||
                (S_ISCHR(filebuf.st_mode) && filetype == CHAR_FILE) ||
                (S_ISBLK(filebuf.st_mode) && filetype == BLOCK_FILE) ||
                (S_ISFIFO(filebuf.st_mode) && filetype == FIFO_FILE) ||
                (S_ISLNK(filebuf.st_mode) && filetype == LINK_FILE) ||
                (S_ISSOCK(filebuf.st_mode) && filetype == SOCKET_FILE))
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