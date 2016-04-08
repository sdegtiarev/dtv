#include <QtGui/QApplication>
#include "dtv.h"


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);

	dtv::widget x;
	QObject::connect(&x, SIGNAL(quit()), qApp, SLOT(quit()));
	std::vector<std::string> files;
	for(int i=1; i < argc; ++i)
		files.push_back(argv[i]);
	x.load(files);
	//x.sample();
	x.show();

	return app.exec();
}
