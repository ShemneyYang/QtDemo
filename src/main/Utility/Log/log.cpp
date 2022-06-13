#include "log.h"
#include <iostream>
#include <assert.h>
#include <intrin.h>

namespace Utility
{
	BOOL GetModuleNameByAddr(DWORD dwAddr, wchar_t* szName, UINT cch)
	{
		typedef struct _PEB_LDR_DATA {                          // Size = 0x24
			ULONG           Length;                             // 00
			BOOLEAN         Initialized;                        // 04
			PVOID           SsHandle;                           // 08
			LIST_ENTRY      InLoadOrderModuleList;              // 0C
			LIST_ENTRY      InMemoryOrderModuleList;            // 14
			LIST_ENTRY      InInitializationOrderModuleList;    // 1C
			//  void *          EntryInProgress;                // 24
		} PEB_LDR_DATA, * PPEB_LDR_DATA;


		szName[0] = 0;

		void* PEB = NULL,
			* Flink = NULL,
			* p = NULL;
		PEB_LDR_DATA* pLdr = NULL;
		__asm
		{
			push edx
			mov edx, fs: [0x30]
			mov PEB, edx
			pop edx
		}
		pLdr = *(PEB_LDR_DATA**)((char*)PEB + 0x0c);
		Flink = &pLdr->InLoadOrderModuleList;
		p = pLdr->InLoadOrderModuleList.Flink;

		__try
		{
			do
			{
				void* pBaseAddress = *(void**)((char*)p + 0x18);
				assert(pBaseAddress && ((DWORD)pBaseAddress & 0xFFFF) == 0);
				if (!pBaseAddress)
					break;

				wchar_t* lpwszFullName = *((wchar_t**)((unsigned char*)p + 0x28));
				DWORD dwImageSize = *((DWORD*)((unsigned char*)p + 0x20));

				if (dwAddr >= (DWORD)pBaseAddress && dwAddr < (DWORD)pBaseAddress + dwImageSize)
				{
					wchar_t* lpwszName = lpwszFullName;
					for (int i = 0; lpwszFullName[i] != 0; i++)
					{
						if (lpwszFullName[i] == L'\\')
							lpwszName = lpwszFullName + i + 1;
					}

					//::WideCharToMultiByte(CP_ACP, 0, lpwszName, -1, szName, cch, NULL, NULL);
					int nLen = wcslen(lpwszName);
					memcpy_s(szName, cch * 2, lpwszName, nLen * 2);

					break;
				}

				p = *((void**)p);
			} while (Flink != p);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			assert(0);
		}

		return szName[0] != 0;
	}

	LogString::LogString(int nLogLevel, const wchar_t* pFilter, const wchar_t* pCppName,
		int nLineNumber, const wchar_t* pFuncName)
		:m_nLevel(nLogLevel)
		, m_nLiveNumber(nLineNumber)
	{
		if (NULL != pFilter)
		{
			m_strFilter = pFilter;
		}

		if (NULL != pCppName)
		{
			m_strCppName = pCppName;
		}

		if (NULL != pFuncName)
		{
			m_strFuncName = pFuncName;
		}
	}

	LogString::~LogString(void)
	{
		wchar_t szModuleName[MAX_PATH] = { 0 };
		GetModuleNameByAddr((DWORD)_ReturnAddress(), szModuleName, _countof(szModuleName));
		LogFile::GetInstance()->doLog(m_nLevel, m_strFilter, m_strCppName, m_nLiveNumber, m_strFuncName, szModuleName, m_strContent);
	}

	LogString& LogString::operator << (const bool b)
	{
		m_strContent += ((b) ? L"true" : L"false");
		return *this;
	}

	LogString& LogString::operator << (const char ch)
	{
		wchar_t wc = ch;
		m_strContent += wc;
		return *this;
	}

	LogString& LogString::operator << (const signed short i)
	{
		return this->operator<<((signed int)i);
	}

	LogString& LogString::operator << (const unsigned short i)
	{
		return this->operator<<((unsigned int)i);
	}

	LogString& LogString::operator << (const signed int i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%d", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const unsigned int i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%u", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const float i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%f", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const unsigned long i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%lu", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const signed long i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%ld", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const __int64 i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%lld", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const unsigned __int64 i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%llu", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const double i)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"%f", i);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const void* ptr)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"0x%x", (__int64)ptr);
		m_strContent += wcBuffer;
		return *this;
	}

	LogString& LogString::operator << (const std::string& str)
	{
		int nCharLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
		if (0 < nCharLen)
		{
			int nOldSize = m_strContent.size();
			m_strContent.append(nCharLen, 0);
			MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), (wchar_t*)(m_strContent.c_str() + nOldSize), nCharLen);
		}
		return *this;
	}

	LogString& LogString::operator << (const std::wstring& str)
	{
		m_strContent += str;
		return *this;
	}

	LogString& LogString::operator << (const char* str)
	{
		if (NULL == str)
		{
			return *this;
		}

		int nLen = strlen(str);
		if (0 >= nLen)
		{
			return *this;
		}

		int nCharLen = MultiByteToWideChar(CP_ACP, 0, str, nLen, NULL, 0);
		if (0 >= nCharLen)
		{
			return *this;
		}

		int nOldSize = m_strContent.size();
		m_strContent.append(nCharLen, 0);
		MultiByteToWideChar(CP_ACP, 0, str, nLen, (wchar_t*)(m_strContent.c_str() + nOldSize), nCharLen);
		return *this;
	}

	LogString& LogString::operator << (const wchar_t* str)
	{
		if (NULL == str)
		{
			return *this;
		}

		m_strContent += str;
		return *this;
	}

#ifdef QT_DLL
	LogString& LogString::operator << (const QString& str)
	{
		if (str.isEmpty())
		{
			return *this;
		}

		m_strContent += str.utf16();
		return *this;
	}
#endif

	LogFile::LogFile(void)
		: m_outputFlag(OPF_Console)
	{

	}

	LogFile::~LogFile(void)
	{

	}

	LogFile* LogFile::GetInstance(void)
	{
		static LogFile* g_pLogFile = NULL;
		if (NULL == g_pLogFile)
		{
			g_pLogFile = new LogFile();
		}
		return g_pLogFile;
	}

	void LogFile::init(const wchar_t* pFilePath, TOutputFlag flag)
	{
		m_outputFlag = flag;
		setFilePath(pFilePath);
	}

	void LogFile::doLog(int nLogLevel, const std::wstring& strFilter, const std::wstring& strCppName,
		int nLineNumber, const std::wstring& strFuncName, const std::wstring& strDllName, const std::wstring& strContent)
	{
		CAutoLocker autoLock(&m_lock);

		if (m_outputFlag & OPF_Console)
		{
			std::wstring strLog;
			getOutputString(nLogLevel, strFilter, strCppName, nLineNumber, strFuncName, strDllName, strContent, strLog);
			std::wcout << strLog.c_str() << std::endl;
		}

		if (m_outputFlag & OPF_OutputDebugString)
		{
			std::wstring strLog;
			getOutputString(nLogLevel, strFilter, strCppName, nLineNumber, strFuncName, strDllName, strContent, strLog);
			OutputDebugStringW(strLog.c_str());
			return;
		}
	}

	void LogFile::setFilePath(const wchar_t* pFilePath)
	{
		if (NULL == pFilePath)
		{
			return;
		}
		m_strFilePath = pFilePath;
	}

	unsigned int LogFile::doLogThreadProc(void* param)
	{
		return 0;
	}

	void LogFile::getOutputString(int nLogLevel, const std::wstring& strFilter, const std::wstring& strCppName,
		int nLineNumber, const std::wstring& strFuncName, const std::wstring& strDllName,
		const std::wstring& strContent, std::wstring& strOut)
	{
		wchar_t wcBuffer[128] = { 0 };
		swprintf_s(wcBuffer, 128, L"Level=%d ", nLogLevel);

		strOut += wcBuffer;
		strOut += strFilter;
		strOut += L" ";
		strOut += strDllName;
		strOut += L" ";
		strOut += strContent;
		strOut += L" ";
		strOut += strFuncName;
		strOut += L" ";
		strOut += strCppName;
		swprintf_s(wcBuffer, 128, L"(%d)\n", nLineNumber);
		strOut += wcBuffer;
	}
}