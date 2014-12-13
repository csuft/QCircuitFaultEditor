#include "circuitfaulteditor.h"
#include <QtWidgets/QApplication>
#include <QFile>
#include <QSharedMemory>

#include "CustomCtrl/msgbox.h"
#include "customCtrl/loginDlg.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	int retVal;
	QFile file(":/styles/global");
	file.open(QFile::ReadOnly);
	qApp->setStyleSheet(file.readAll());
	file.close();

	// 使用共享内存来保证程序单实例运行
	QSharedMemory sharedMemory("CircuitFaultEditor");
	if (sharedMemory.create(1) && sharedMemory.error() != QSharedMemory::AlreadyExists)
	{
		LoginDlg dlg;
		if (dlg.exec() == QDialog::Accepted)
		{
			CircuitFaultEditor w;
			w.show();
			retVal = app.exec();
		}
	}
	else
	{
		CMessageBox msgBox;
		msgBox.setInformation(QStringLiteral("警告"), QStringLiteral("警告：本程序实例已经运行！"), QPixmap(":/pics/warning"));
		msgBox.exec();
	}

	return retVal;
}
