#include "File.h"
#include <shlobj_core.h>
#include <shlwapi.h>
#include <time.h>
#include <string>

namespace Utility
{
	bool File::readFile(const wchar_t* pFile, std::string& strData)
	{
		if (NULL == pFile)
		{
			strData.clear();
			return false;
		}
		
		long long nFileSize = getFileSize(pFile);
		if (0 >= nFileSize)
		{
			strData.clear();
			return false;
		}

		if (0 == nFileSize)
		{
			strData.clear();
			return true;
		}

		const int nMaxSize = 16 * 1024 * 1024;
		if (nMaxSize < nFileSize)
		{
			//超过16MB的文件，不支持。
			return false;
		}

		FILE* pOpenFile = NULL;
		_wfopen_s(&pOpenFile, pFile, L"rb");
		if (NULL != pOpenFile)
		{
			strData.resize((size_t)nFileSize);
			int nReadCount = fread((void*)strData.data(), 1, strData.size(), pOpenFile);
			fclose(pOpenFile);

			if (nReadCount == strData.size())
			{
				return true;
			}
		}

		strData.clear();
		return false;
	}

	bool File::writeFile(const wchar_t* pFile, const std::string& strData)
	{
		if (NULL == pFile)
		{
			return false;
		}

		const int nMaxSize = 16 * 1024 * 1024;
		if (nMaxSize < strData.size())
		{
			//超过16MB的文件，不支持。
			return false;
		}

		FILE* pOpenFile = NULL;
		_wfopen_s(&pOpenFile, pFile, L"wb+");
		if (NULL != pOpenFile)
		{
			int nWriteCount = fwrite(strData.data(), 1, strData.size(), pOpenFile);
			fclose(pOpenFile);

			if (nWriteCount == strData.size())
			{
				return true;
			}
		}

		return false;
	}

	bool File::isFileExist(const wchar_t* pFile)
	{
		if (NULL == pFile)
		{
			return false;
		}

		return (0 == _waccess_s(pFile, 0));
	}

	bool File::checkDirectoryExist(const wchar_t* pFile)
	{
		return isFileExist(pFile);
	}

	bool File::createAllDirectory(const wchar_t* pFile)
	{
		return (ERROR_SUCCESS == SHCreateDirectory(NULL, pFile));
	}

	//强制删除文件或是删除一个空目录
	bool File::deleteFileOrDirectory(const wchar_t* pFile)
	{
		int nRet = 0;
		if (!isFileExist(pFile))
		{
			return true;
		}
		
		std::wstring strFile = pFile;

		SHFILEOPSTRUCT sfDelete;
		memset(&sfDelete, 0, sizeof(sfDelete));
		sfDelete.hwnd = NULL;
		sfDelete.wFunc = FO_DELETE;
		sfDelete.pFrom = strFile.c_str();
		sfDelete.pTo = strFile.c_str();
		sfDelete.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
		nRet = SHFileOperation(&sfDelete);
		if (0 != nRet)
		{
			const TCHAR C_NAME_REFIX[] = { L"{6B00FE91-95C1-470e-870C-509748F5F3D4}" };

			std::wstring strFile(pFile);
			strFile += C_NAME_REFIX;

			srand((unsigned int)time(NULL));

			for (int i = 0; i < 10 && 0 != nRet; ++i)
			{//删除操作失败,可能是被占用等,尝试去重命名文件或者目录.
				wchar_t bufRand[128] = { 0 };
				wnsprintf(bufRand, _countof(bufRand) - 1, L"%ld", rand());
				strFile += bufRand;

				std::wstring strFileTo = strFile;

				SHFILEOPSTRUCT sfDelete;
				memset(&sfDelete, 0, sizeof(sfDelete));
				sfDelete.hwnd = NULL;
				sfDelete.wFunc = FO_RENAME;
				sfDelete.pFrom = strFile.c_str();
				sfDelete.pTo = strFileTo.c_str();
				sfDelete.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
				nRet = SHFileOperation(&sfDelete);
			}
		}

		if (0 != nRet)
		{
			DWORD dwAttrs = GetFileAttributes(pFile);
			dwAttrs &= (~FILE_ATTRIBUTE_READONLY);
			dwAttrs &= (~FILE_ATTRIBUTE_HIDDEN);
			SetFileAttributes(pFile, dwAttrs);

			if (!::DeleteFile(pFile))
			{
				if (::RemoveDirectory(pFile))
				{
					nRet = 0;
				}
			}
			else
			{
				nRet = 0;
			}
		}

		return (0 == nRet);
	}

	long long File::getFileSize(const wchar_t* pFile)
	{
		if (NULL == pFile)
		{
			return -1;
		}

		HANDLE hFileHandle = CreateFile(pFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFileHandle)
		{
			return -1;
		}

		LARGE_INTEGER filesize;
		if (GetFileSizeEx(hFileHandle, &filesize))
		{
			::CloseHandle(hFileHandle);
			return filesize.QuadPart;
		}

		::CloseHandle(hFileHandle);
		return -1;
	}
}