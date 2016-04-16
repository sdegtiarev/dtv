#ifndef DTV_H
#define DTV_H
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <qwt_legend.h>
#include <qwt_plot_renderer.h>
#include <QtGui/QWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QFrame>
#include <QtGui/QKeyEvent>
#include <QtCore/QTimer>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "loader.h"


namespace dtv {

class notes;
class board;
class graph;
enum SCALE { SCALE_INDIVIDUAL, SCALE_SHARED, SCALE_ASME };

class widget : public QwtPlot
{
Q_OBJECT
public:
	widget(QWidget* =0);
	~widget() {}
	void sample();
	void load(std::vector<std::string>);
	void title(std::string s) { setWindowTitle(s.c_str()); }
	void memo(int);
	void xlabel(std::string);
	void ylabel(std::string);
	void palette(std::string s) { _palette=s; }
	int status() const { return st; }
	
signals:
	void quit();
	
private slots:
	void check() {
		for(auto pl=ld->get(); pl; pl=ld->get())
		{
			pl->attach(this);
			replot();
		}
	}

protected:
	void keyPressEvent(QKeyEvent*);
	
private:
	QTimer _timer;
	std::unique_ptr<loader> ld;
	std::string _palette;
	int st;

	void plain_key_event(QKeyEvent*);
	void ctrl_key_event(QKeyEvent*);
};


class board : public QFrame
{
Q_OBJECT
public:
	board(QWidget*);
};


}
#endif

