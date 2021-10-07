#include "stable.h"
#include "mainframe.h"

const quint32 KDataDirName = Qt::UserRole + 1;

MainFrame::MainFrame(QWidget *parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
	m_ui.setupUi(this);
	
	Qt::WindowFlags flags = this->windowFlags();
	flags &= ~Qt::WindowContextHelpButtonHint;
	flags |= Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint;
	setWindowFlags(flags);

	m_ui.textEditPath->setWordWrapMode(QTextOption::WrapAnywhere);

	TCHAR buffer[MAX_PATH] = {0};
	SHGetSpecialFolderPath(NULL,buffer,CSIDL_APPDATA,NULL);
	m_strComstorePath = QString::fromStdWString(buffer) + "\\duowan\\yy\\yycomstore\\2052";
	m_strGameShowTemplateTipsFile = QString::fromStdWString(buffer) + "\\duowan\\yy\\business\\gameshow\\showTestTips";
	m_strEntTemplateTipsFile = QString::fromStdWString(buffer) + "\\duowan\\yy\\business\\yymusicconfig\\4_5\\showTestTips";
	m_strVideosdkShowD = QString::fromStdWString(buffer) + "\\duowan\\yy\\log\\videosdk\\showd";

	_readHistoryPath();
}

MainFrame::~MainFrame(void)
{
	
}

void MainFrame::on_btnFresh_clicked(void)
{
	
}

void MainFrame::on_btnFreshVersonList_clicked(void)
{
	
}

void MainFrame::on_btnLink_clicked(void)
{
	QString strDstTargetPath = m_ui.comboBoxTargetPath->currentText();
	if (strDstTargetPath.isEmpty())
	{
		return;
	}
	strDstTargetPath.replace("/","\\");
	
	QString strDirPath = m_ui.textEditPath->toPlainText();
	QString strOldTargetPath = _getLinkTarget(strDirPath);
	if (strOldTargetPath == strDstTargetPath)
	{
		return;
	}

	if (strOldTargetPath.isEmpty())
	{
		std::wstring strSrcDirPath = strDirPath.toStdWString();
		std::wstring strDstDirPath = strSrcDirPath + L".bak";
		BOOL bRet = MoveFile(strSrcDirPath.c_str(),strDstDirPath.c_str());
		if (!bRet)
		{
			DWORD dwError = ::GetLastError();
			QString strText = QString("Failed to MoveFile src file=%1,dst file=%2,last error=%3").arg(strDirPath).
				arg(strDirPath + ".bak").arg(dwError);
			::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
			return;
		}
		
		bRet = CreateSymbolicLink(strDirPath.toStdWString().c_str(),strDstTargetPath.toStdWString().c_str(),0x1);
		if (!bRet)
		{
			DWORD dwError = ::GetLastError();
			QString strText = QString("Failed to CreateSymbolicLink link name=%1,target file=%2,last error=%3").arg(strDirPath).
				arg(strDstTargetPath).arg(dwError);
			::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
			return;
		}
		_copyMd5File(QString::fromStdWString(strDstDirPath),QString::fromStdWString(strSrcDirPath));
	}
	else
	{
		QString strDirBakPath = strDirPath + ".bak";
		QDir dir(strDirBakPath);
		if (!dir.exists())
		{
			QString strText = "The file isn't existsed,file=" + strDirBakPath;
			::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
			return;
		}
		//删除已有的连接文件夹.
		BOOL bRet = RemoveDirectory(strDirPath.toStdWString().c_str());
		if (!bRet)
		{
			DWORD dwError = ::GetLastError();
			QString strText = QString("Failed to RemoveDirectory path=%1,last error=%3").arg(strDirPath).arg(dwError);
			::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
			return;
		}
		bRet = CreateSymbolicLink(strDirPath.toStdWString().c_str(),strDstTargetPath.toStdWString().c_str(),0x1);
		if (!bRet)
		{
			DWORD dwError = ::GetLastError();
			QString strText = QString("Failed to CreateSymbolicLink link name=%1,target file=%2,last error=%3").arg(strDirPath).
				arg(strDstTargetPath).arg(dwError);
			::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
			return;
		}
		_copyMd5File(strDirBakPath,strDirPath);
	}

	_saveHistoryPath();
	_readHistoryPath();
}

void MainFrame::on_btnOpenPath_clicked(void)
{
	QString strDstTargetPath = m_ui.comboBoxTargetPath->currentText();
	QString	strDir = QFileDialog::getExistingDirectory(this,QString::fromStdWString(L"选择文件夹"),strDstTargetPath);
	if (strDir.isEmpty())
	{
		return;
	}
	strDir.replace("/","\\");
	m_ui.textEditPath->setText(strDir);
}

void MainFrame::on_btnOpenTarget_clicked()
{
	QString strDstTargetPath = m_ui.comboBoxTargetPath->currentText();
	QString	strDir = QFileDialog::getExistingDirectory(this,QString::fromStdWString(L"选择目标文件夹"),strDstTargetPath);
	if (strDir.isEmpty())
	{
		return;
	}
	strDir.replace("/","\\");
	m_ui.comboBoxTargetPath->setEditText(strDir);
}

void MainFrame::on_btnRevert_clicked(void)
{
	QString strDirPath = m_ui.textEditPath->toPlainText();
	QString strDirBakPath = strDirPath + ".bak";
	QString strTargetPath = _getLinkTarget(strDirPath);
	if (strTargetPath.isEmpty())
	{
		QString strText = QString("Isn't link dir !");
		::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
		return;
	}
	QDir bakDir(strDirBakPath);
	if (!bakDir.exists())
	{
		QString strText = QString("Version bak dir not found !");
		::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
		return;
	}


	BOOL bRet = RemoveDirectory(strDirPath.toStdWString().c_str());
	if (!bRet)
	{
		DWORD dwError = ::GetLastError();
		QString strText = QString("Failed to RemoveDirectory path=%1,last error=%3").arg(strDirPath).arg(dwError);
		::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
		return;
	}

	bRet = MoveFile(strDirBakPath.toStdWString().c_str(),strDirPath.toStdWString().c_str());
	if (!bRet)
	{
		DWORD dwError = ::GetLastError();
		QString strText = QString("Failed to MoveFile src file=%1,dst file=%2,last error=%3").arg(strDirPath).
			arg(strDirPath + ".bak").arg(dwError);
		::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
		return;
	}
}

void MainFrame::on_btnDelete_clicked(void)
{

}

void MainFrame::on_checkBoxGameShowTemplateTips_clicked(bool checked)
{
	if (checked)
	{
		_writeFileData(m_strGameShowTemplateTipsFile.toStdWString().c_str(), "123");
	}
	else
	{
		_deleteFileOrDirectory(m_strGameShowTemplateTipsFile.toStdWString().c_str());
	}
}

void MainFrame::on_checkBoxEntTemplateTips_clicked(bool checked)
{
	if (checked)
	{
		_writeFileData(m_strEntTemplateTipsFile.toStdWString().c_str(), "123");
	}
	else
	{
		_deleteFileOrDirectory(m_strEntTemplateTipsFile.toStdWString().c_str());
	}
}

void MainFrame::on_checkBoxShowD_clicked(bool checked)
{
	if (checked)
	{
		_writeFileData(m_strVideosdkShowD.toStdWString().c_str(), "123");
	}
	else
	{
		_deleteFileOrDirectory(m_strVideosdkShowD.toStdWString().c_str());
	}
}

void MainFrame::on_cbbTemplateName_currentIndexChanged(const QString & text)
{

}

void MainFrame::on_lwVersiones_itemClicked(QListWidgetItem * item)
{
	QString strDir = item->data(KDataDirName).toString();
	if (strDir.isEmpty())
	{
		return;
	}
	QString strSrcDir = m_strComstorePath + "\\" + m_strCurretTemplateName + "\\" + strDir;
	m_ui.textEditPath->setText(strSrcDir);

	QString strTargetPath = _getLinkTarget(strSrcDir);
	if (strTargetPath.isEmpty())
	{
		return;
	}
	m_ui.comboBoxTargetPath->setEditText(strTargetPath);
}

void MainFrame::on_lwVersiones_itemDoubleClicked(QListWidgetItem * item)
{
	QString strDir = item->data(KDataDirName).toString();
	if (strDir.isEmpty())
	{
		return;
	}
	QString strPath = m_strComstorePath + "\\" + m_strCurretTemplateName + "\\" + strDir;
	ShellExecute(NULL,L"open",strPath.toStdWString().c_str(),NULL,NULL,SW_SHOWNORMAL);
}

unsigned __stdcall MainFrame::doWatchDirThread(void *vparam)
{
	if (NULL == vparam)
	{
		return 0;
	}
	MainFrame* frame = (MainFrame*)vparam;
	frame->_onWatchingDir();

	return 0;
}

void MainFrame::_onWatchingDir(void)
{
	// Watch the directory for file creation and deletion.
	HANDLE hWacth = FindFirstChangeNotification( 
		L"G:\\Users\\Administrator\\AppData\\Roaming\\duowan\\yy\\yycomstore\\2052\\com.yy.template.entertainment",//m_strWatchDirPath.toStdWString().c_str(),// directory to watch 
		FALSE,                         // do not watch subtree 
		FILE_NOTIFY_CHANGE_DIR_NAME); // watch file name changes 

	if (hWacth == INVALID_HANDLE_VALUE) 
	{
		return;
	}


	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 

	while (TRUE) 
	{ 
		// Wait for notification.
		//DWORD dwWaitStatus = WaitForSingleObject(hWacth,INFINITE); 
		DWORD dwWaitStatus = 0;
		wchar_t wcBuffer[1024] = {0};
		DWORD dwRetSize = 0;
		ReadDirectoryChangesW(hWacth,wcBuffer,1024,FALSE,FILE_NOTIFY_CHANGE_DIR_NAME,&dwRetSize,NULL,NULL);
		FILE_NOTIFY_INFORMATION* pInfo = (FILE_NOTIFY_INFORMATION*)wcBuffer;

		switch (dwWaitStatus) 
		{ 
		case WAIT_OBJECT_0: 
			{
				// A file was created, renamed, or deleted in the directory.
				// Refresh this directory and restart the notification.
				//RefreshDirectory(lpDir); 
				if ( FindNextChangeNotification(hWacth) == FALSE )
				{
					return;
				}
			}
			break; 

		case WAIT_OBJECT_0 + 1: 

			// A directory was created, renamed, or deleted.
			// Refresh the tree and restart the notification.

			//RefreshTree(lpDrive); 
			if (FindNextChangeNotification(hWacth) == FALSE )
			{
				return;
			}
			break; 

		case WAIT_TIMEOUT:

			// A timeout occurred, this would happen if some value other 
			// than INFINITE is used in the Wait call and no changes occur.
			// In a single-threaded environment you might not want an
			// INFINITE wait.

			//printf("\nNo changes in the timeout period.\n");
			break;

		default: 
			//printf("\n ERROR: Unhandled dwWaitStatus.\n");
			//ExitProcess(GetLastError());
			break;
		}
	}

	FindCloseChangeNotification(hWacth);
}

QString MainFrame::_getLinkTarget(const QString& strDirPath)
{
	std::wstring wsPath = strDirPath.toStdWString();
	HANDLE hFile = CreateFile(wsPath.c_str(),
		FILE_READ_ATTRIBUTES,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		0,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_ATTRIBUTE_REPARSE_POINT,
		0);
	if (NULL == hFile)
	{
		return "";
	}

	wchar_t wcBuffer[128] = {0}; 
	DWORD dwRet = GetFinalPathNameByHandle(hFile,wcBuffer,128,FILE_NAME_OPENED);
	if (dwRet <= 0)
	{
		CloseHandle(hFile);
		return "";
	}
	CloseHandle(hFile);

	QString strTargetPath = QString::fromStdWString(wcBuffer);
	if (!strTargetPath.startsWith("\\\\?\\"))
	{
		return "";
	}
	strTargetPath.remove(0,4);
	if (strTargetPath == strDirPath)
	{
		return "";
	}
	return strTargetPath;
}

void MainFrame::_copyMd5File(const QString& strSrcDir,const QString& strDstDir)
{
	QDir dir(strSrcDir);
	QStringList nameList = dir.entryList(QDir::Files|QDir::NoDot|QDir::NoDotDot,QDir::Name);
	for(int i = 0; i < nameList.count(); i++)
	{
		const QString& strName = nameList.at(i);
		if (!strName.startsWith('~'))
		{
			continue;
		}
		QString strSrcFile = strSrcDir + "\\" + strName;
		QString strDstFile = strDstDir + "\\" + strName;
		BOOL bRet = CopyFile(strSrcFile.toStdWString().c_str(),strDstFile.toStdWString().c_str(),FALSE);
		if (!bRet)
		{
			DWORD dwError = ::GetLastError();
			QString strText = QString("Failed to CopyFile src file=%1,dst file=%2,last error=%3").arg(strSrcFile).
				arg(strDstFile).arg(dwError);
			::MessageBox(NULL,strText.toStdWString().c_str(),L"System Error",MB_OK|MB_ICONERROR);
		}
	}
}

void MainFrame::_startWatchDirThread(const QString& strDir)
{
	m_strWatchDirPath = strDir;
	_beginthreadex(NULL,0,doWatchDirThread,this,0,NULL);
}

void MainFrame::_stopWatchDirThread(void)
{

}

void MainFrame::_readHistoryPath(void)
{
	QSettings setting("set.ini",QSettings::IniFormat);
	setting.beginGroup("historyPath");
	QString strList = setting.value("list").toString();
	setting.endGroup();
	if (strList.isEmpty())
	{
		return;
	}
	m_strLinkTargetPathList.clear();
	m_strLinkTargetPathList = strList.split(";");

	m_ui.comboBoxTargetPath->clear();
	m_ui.comboBoxTargetPath->addItems(m_strLinkTargetPathList);
}

void MainFrame::_saveHistoryPath(void)
{
	QSet<QString> strSet;
	QString strSave;
	QString strText = m_ui.comboBoxTargetPath->currentText();
	if (!strText.isEmpty())
	{
		strSet.insert(strText);
		strSave.append(strText);
	}
	int nCount = m_ui.comboBoxTargetPath->count();
	for (int i = 0; i < nCount; ++i)
	{
		strText = m_ui.comboBoxTargetPath->itemText(i);
		if (strText.isEmpty())
		{
			continue;
		}
		if (!strSet.contains(strText))
		{
			strSet.insert(strText);
			strSave.append(";");
			strSave.append(strText);
		}
	}
	QSettings setting("set.ini",QSettings::IniFormat);
	setting.beginGroup("historyPath");
	setting.setValue("list", strSave);
	setting.endGroup();
}

bool MainFrame::_deleteFileOrDirectory(LPCTSTR filepath)
{
	int nRet = 0;

	if(PathFileExists(filepath))
	{
		TCHAR szBuffFrom[MAX_PATH +2] ={0};
		_tcsncpy(szBuffFrom, filepath,sizeof(szBuffFrom)/sizeof(szBuffFrom[0]) -2*sizeof(szBuffFrom[0]));

		SHFILEOPSTRUCT sfDelete;
		memset(&sfDelete, 0, sizeof(sfDelete));
		sfDelete.hwnd = NULL;
		sfDelete.wFunc = FO_DELETE;
		sfDelete.pFrom = szBuffFrom;
		sfDelete.pTo   = szBuffFrom;
		sfDelete.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
		nRet = SHFileOperation(&sfDelete);
		if(0 != nRet)
		{
			const TCHAR C_NAME_REFIX[] = {_T("{6B00FE91-95C1-470e-870C-509748F5F3D4}")};

			std::wstring strFile(filepath);
			strFile += C_NAME_REFIX;

			srand((unsigned int)time(NULL));

			for (int i = 0; i < 10 && 0 != nRet; ++i)
			{//删除操作失败,可能是被占用等,尝试去重命名文件或者目录.
				TCHAR bufRand[128] ={0};
				wnsprintf(bufRand, _countof(bufRand) -1,_T("%ld"), rand());
				strFile += bufRand;

				TCHAR szBuffTo[MAX_PATH +2] ={0};
				_tcsncpy(szBuffTo,  strFile.c_str(),_countof(szBuffTo) -1);

				SHFILEOPSTRUCT sfDelete;
				memset(&sfDelete, 0, sizeof(sfDelete));
				sfDelete.hwnd = NULL;
				sfDelete.wFunc = FO_RENAME;
				sfDelete.pFrom = szBuffFrom;
				sfDelete.pTo   = szBuffTo;
				sfDelete.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
				nRet = SHFileOperation(&sfDelete);
			}	
		}

		if(0 != nRet)
		{
			DWORD dwAttrs  = GetFileAttributes(filepath); 
			dwAttrs &= (~FILE_ATTRIBUTE_READONLY);
			dwAttrs &= (~FILE_ATTRIBUTE_HIDDEN);
			SetFileAttributes(filepath, dwAttrs);

			if(!::DeleteFile(filepath))
			{
				if(::RemoveDirectory(filepath))
				{
					nRet = 0;
				}
			}
			else
			{
				nRet = 0;
			}
		}
	}

	return 0 == nRet;
}

bool MainFrame::_writeFileData(const std::wstring &fileName, const std::string &data)
{
	FILE *fp = _wfopen(fileName.c_str(), L"wb+");
	if(fp)
	{
		size_t count = fwrite(data.data(), 1, data.size(), fp);
		fclose(fp);

		if(count == data.size())
		{
			return true;
		}
	}

	return false;
}