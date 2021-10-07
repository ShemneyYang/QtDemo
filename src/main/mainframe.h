#pragma once

#include "ui_mainframe.h"

class QListWidgetItem;
class MainFrame : public QDialog
{
	Q_OBJECT

public:
	MainFrame(QWidget *parent = 0, Qt::WindowFlags f = 0);
	virtual ~MainFrame(void);

protected slots:
	void on_btnFresh_clicked(void);
	void on_btnFreshVersonList_clicked(void);
	void on_btnLink_clicked(void);
	void on_btnOpenPath_clicked(void);
	void on_btnOpenTarget_clicked(void);
	void on_btnRevert_clicked(void);
	void on_btnDelete_clicked(void);
	void on_checkBoxGameShowTemplateTips_clicked(bool checked);
	void on_checkBoxEntTemplateTips_clicked(bool checked);
	void on_checkBoxShowD_clicked(bool checked);
	void on_cbbTemplateName_currentIndexChanged(const QString & text);
	void on_lwVersiones_itemClicked(QListWidgetItem * item);
	void on_lwVersiones_itemDoubleClicked(QListWidgetItem * item);

protected:
	static unsigned __stdcall doWatchDirThread(void *param);

private:
	void _onWatchingDir(void);
	QString _getLinkTarget(const QString& strDirPath);
	void _copyMd5File(const QString& strSrcDir,const QString& strDstDir);
	void _startWatchDirThread(const QString& strDir);
	void _stopWatchDirThread(void);
	void _readHistoryPath(void);
	void _saveHistoryPath(void);
	bool _deleteFileOrDirectory(LPCTSTR filepath);
	bool _writeFileData(const std::wstring &fileName, const std::string &data);

private:
	Ui::Dialog m_ui;

	QString m_strCurretTemplateName;
	QString m_strWatchDirPath;
	QString m_strComstorePath;
	QString m_strGameShowTemplateTipsFile;
	QString m_strEntTemplateTipsFile;
	QString m_strVideosdkShowD;
	QStringList m_strLinkTargetPathList;
};
