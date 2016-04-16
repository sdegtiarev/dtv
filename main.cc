#include <QtGui/QApplication>
#include "dtv.h"


int main(int argc, char *argv[])
{
	QApplication app(argc,argv);
	dtv::widget x;

	int opt;
	while((opt=getopt(argc,argv,"t:x:y:L:C:")) != -1)
	switch(opt) {
		case 't': x.title(optarg); break;
		case 'x': x.xlabel(optarg); break;
		case 'y': x.ylabel(optarg); break;
		case 'L': x.memo(atol(optarg)); break;
		case 'C': x.palette(optarg); break;
		default : return 1;
	}

	QObject::connect(&x, SIGNAL(quit()), qApp, SLOT(quit()));
	std::vector<std::string> files;
	for(int i=optind; i < argc; ++i)
		files.push_back(argv[i]);
	x.load(files);
	//x.sample();
	x.show();

	app.exec();
	return x.status();
}
