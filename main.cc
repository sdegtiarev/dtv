#include <QtGui/QApplication>
#include "dtv.h"


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	dtv::widget x;
	QObject::connect(&x, SIGNAL(quit()), qApp, SLOT(quit()));
	//x.sample();
	x.show();

	return app.exec();
}
