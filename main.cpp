#include "mainwindow.h"
#include <QApplication>
// #include <QCoreApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
  //  QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar, true);
	MainWindow w;
	w.show();

	return a.exec();
}
