#include "dtv.h"
#include <unistd.h>
#include <thread>
#include <QtGui/QPainter>
#include <QtGui/QPrinter>
#include <qwt_plot_grid.h>




dtv::widget::widget(QWidget* parent)
: QwtPlot(parent)
{
	setWindowTitle(" ");
	setPalette(QColor(Qt::white));
	setAutoFillBackground(true);
	insertLegend(new QwtLegend(this), QwtPlot::BottomLegend);
	auto grid=new QwtPlotGrid();
	grid->attach(this);

	connect(&_timer, SIGNAL(timeout()), this, SLOT(check()));
}


void dtv::widget::sample()
{
	QwtPlotCurve *cv=new QwtPlotCurve("sin plot");
	cv->setTitle("sin");
	cv->setPen(QColor("black"));
	double x[1000], y[1000];
	for(int i=0; i < 1000; ++i) { x[i]=i*.01; y[i]=(1+sin(x[i]))/2; }
	cv->setSamples(x, y, 1000);
	cv->attach(this);
}



void dtv::widget::keyPressEvent(QKeyEvent* e)
{
	e->accept();
	bool mode=e->modifiers() & Qt::ControlModifier;
	if(mode) ctrl_key_event(e);
	else plain_key_event(e);
}


void dtv::widget::plain_key_event(QKeyEvent* e)
{
	switch(e->key()) {
/*
		case Qt::Key_Left: _board->show_prev(); break;
		case Qt::Key_Right: _board->show_next(); break;
		case Qt::Key_Insert: _board->mark_checked(); break;
		case Qt::Key_Delete: _board->mark_unchecked(); break;
		case Qt::Key_PageUp: _board->show_prev_checked(); break;
		case Qt::Key_PageDown: _board->show_next_checked(); break;
		case Qt::Key_Home: _board->show_first(); break;
		case Qt::Key_End: _board->show_last(); break;
		case Qt::Key_F1: _board->scale(SCALE_INDIVIDUAL); break;
		case Qt::Key_F2: _board->scale(SCALE_SHARED); break;
		case Qt::Key_F3: _board->scale(SCALE_ASME); break;
		default: e->ignore(); break;
*/
	}
}


void dtv::widget::ctrl_key_event(QKeyEvent* e)
{
	switch(e->key()) {
		case Qt::Key_Q: emit(quit()); update(); break;
/*
		case Qt::Key_Left: _board->show_prev_checked(); break;
		case Qt::Key_Right: _board->show_next_checked(); break;
		case Qt::Key_Insert: _board->mark_checked_all(); break;
		case Qt::Key_Delete: _board->mark_unchecked_all(); break;
		case Qt::Key_M: _board->toggle_checked(); break;
		case Qt::Key_P: _board->print(); break;
		case Qt::Key_1: _board->scale(SCALE_INDIVIDUAL); break;
		case Qt::Key_2: _board->scale(SCALE_SHARED); break;
		case Qt::Key_3: _board->scale(SCALE_ASME); break;
*/
		default: e->ignore(); break;
	}
}


void dtv::widget::load(std::vector<std::string> files)
{
	ld.reset(new loader(files)); 
	std::thread([this]{ this->ld->run(); }).detach();
	_timer.start(100);
}




dtv::board::board(QWidget *parent) 
: QFrame(parent)
{
	setFrameStyle(QFrame::Sunken | QFrame::Panel);
	setMinimumSize(256, 256);
	setPalette(QColor(Qt::white));
	setAutoFillBackground(true);
}

			


