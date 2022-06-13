#pragma once
#include <stdio.h>
#include <string>
#include "../Locker/locker.h"

/*
* Critical 1：所有消息以 Critical 级别或更高级别记录。
* Error 2：所有消息以 Error 级别或更高级别记录。
* Warning 3：所有消息以 Warning 级别或更高级别记录。
* Information 4：所有消息以 Information 级别或更高级别记录。
* Trace 5：所有消息以 Trace 级别或更高级别记录。
* Verbose 5：所有消息以 Trace 级别或更工作流追踪记录。
*/
#define Log1(filter_name)	Utility::LogString(1, L#filter_name, __FILEW__, __LINE__, __FUNCTIONW__)
#define Log2(filter_name)	DWORD dwLogError = GetLastError(); Utility::LogString(2, L#filter_name, __FILEW__, __LINE__, __FUNCTIONW__) << L"GetLastError=" << dwLogError << L","
#define Log3(filter_name)	Utility::LogString(3, L#filter_name, __FILEW__, __LINE__, __FUNCTIONW__)

#define LogInit(strfilePath, OutputTypeFlag)	{Utility::LogFile::GetInstance()->init(strfilePath, OutputTypeFlag);Log1(Info) << L"Log start!!!!!!!file=" << strfilePath << L",flag=" << OutputTypeFlag;}
#define LogInitForConsole LogInit(NULL, Utility::OPF_Console)
#define LogInitForOutputDebugString LogInit(NULL, Utility::OPF_OutputDebugString)
#define LogInitForFile(FilePath) LogInit(FilePath, Utility::OPF_File)

namespace Utility
{
	enum TOutputFlag
	{
		OPF_None = 0,
		OPF_OutputDebugString = 0x01,
		OPF_Console = 0x02,
		OPF_File = 0x04
	};

	class LogString
	{
	public:
		LogString(int nLogLevel, const wchar_t* pFilter, const wchar_t* pCppName,
			int nLineNumber, const wchar_t* pFuncName);
		virtual ~LogString(void);

		LogString& operator << (const bool b);
		LogString& operator << (const char ch);
		LogString& operator << (const signed short i);
		LogString& operator << (const unsigned short i);
		LogString& operator << (const signed int i);
		LogString& operator << (const unsigned int i);
		LogString& operator << (const float f);
		LogString& operator << (const unsigned long i);
		LogString& operator << (const signed long i);
		LogString& operator << (const __int64 i);
		LogString& operator << (const unsigned __int64 i);
		LogString& operator << (const double f);
		LogString& operator << (const void* ptr);
		LogString& operator << (const std::string& str);
		LogString& operator << (const std::wstring& str);
		LogString& operator << (const char* str);
		LogString& operator << (const wchar_t* str);

#ifdef QT_DLL
		LogString& operator << (const QString& str);
#endif

	private:
		DWORD m_nLevel;
		int m_nLiveNumber;
		std::wstring m_strFilter;
		std::wstring m_strCppName;
		std::wstring m_strFuncName;
		std::wstring m_strContent;
	};

	class LogFile
	{
	public:
		LogFile(void);
		virtual ~LogFile(void);

		void init(const wchar_t* pFilePath, TOutputFlag flag);

		void doLog(int nLogLevel, const std::wstring& strFilter, const std::wstring& strCppName,
			int nLineNumber, const std::wstring& strFuncName, const std::wstring& strDllName, const std::wstring& strContent);

		void setFilePath(const wchar_t* pFilePath);

		static LogFile* GetInstance(void);

	protected:
		static unsigned int _stdcall doLogThreadProc(void* param);

	private:
		void getOutputString(int nLogLevel, const std::wstring& strFilter, const std::wstring& strCppName,
			int nLineNumber, const std::wstring& strFuncName, const std::wstring& strDllName, 
			const std::wstring& strContent, std::wstring& strOut);

	private:
		TOutputFlag m_outputFlag;
		std::wstring m_strFilePath;
		CSLock m_lock;
	};

}