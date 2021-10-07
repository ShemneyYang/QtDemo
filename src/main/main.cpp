#include "stable.h"

#include "mainframe.h"

int main(int argc, char *argv[])
{
	CoInitialize(NULL);

	QApplication app(argc, argv);

	MainFrame frame;
	frame.show();	

	int nRet = app.exec();
	CoUninitialize();
	return nRet;
}