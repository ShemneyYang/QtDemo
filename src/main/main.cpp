#include "stable.h"
#include "mainframe.h"

int main(int argc, char *argv[])
{
// 	LogInit(NULL);
// 	Log1(TEST) << "GetCommandLineW=" << GetCommandLineW();
// 	std::string str = "test123456";
// 	Log1(TEST) << str;
	CoInitialize(NULL);

	QApplication app(argc, argv);

	MainFrame frame;
	frame.show();	

	int nRet = app.exec();
	CoUninitialize();
	return nRet;
}