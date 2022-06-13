#pragma once

#include <string>

namespace Utility
{
    class File
    {
    public:
        static bool readFile(const wchar_t* pFile, std::string& strData);
        static bool	writeFile(const wchar_t* pFile, const std::string& strData);

        static bool isFileExist(const wchar_t* pFile);
        static bool checkDirectoryExist(const wchar_t* pFile);
        static bool createAllDirectory(const wchar_t* pFile);

        //强制删除文件或是删除一个空目录
        static bool deleteFileOrDirectory(const wchar_t* pFile);
        static long long getFileSize(const wchar_t* pFile);
    };
}
