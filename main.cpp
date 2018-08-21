#include <QApplication>
#include <QTextCodec>

#include "MainWindow.h"

#include <QApplication>

#include <QSqlDatabase>

#include <QStringList>

#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

#include <QSqlQuery>
#include <QFile>



int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName(app.translate("main", "PGM"));
	app.setOrganizationName("Leoben Ltd.");
	app.setOrganizationDomain("Leoben.cn");
	app.setWindowIcon(QIcon(":images/icon.png"));


	MainWindow view;
	view.show();


	app.exec();

}
