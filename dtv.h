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
	
signals:
	void quit();
	
private slots:
	void check() {
		if(!ld) {
			_timer.stop();
			return;
		}
		for(auto pl=ld.get(); pl; pl=ld.get())
		{
			pl->attach(this);
			replot();
		}
	}

protected:
	void keyPressEvent(QKeyEvent*);
	
private:
	QTimer _timer;
	loader ld;
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

