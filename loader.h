#ifndef DTV_LOADER_H
#define DTV_LOADER_H
#include <unistd.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <sstream>
#include <fstream>



class loader
{
public:
	loader(std::vector<std::string> file)
	: _mtx{std::make_shared<std::mutex>()}
	, _plot{std::make_shared<std::list<QwtPlotCurve*>>()}
	, _file(file)
	{}

	void run() {
		if(_file.empty()) {
			load(std::cin);
			std::cin.clear();
		}
		for(auto file : _file) {
			if(file == "-") {
				load(std::cin);
				std::cin.clear();
			} else {
				std::ifstream f(file);
				load(f);
			}
		}
	}

	void load(std::istream& f) {
		std::string token;
		std::getline(f, token);
		while(f) {
			while(f && parse(token)) {
				title=token;
				std::getline(f, token);
			}
			while(!parse(token) && std::getline(f, token));
			flush_plot();
		}
	}

	operator bool() const { return bool(_plot); }

	QwtPlotCurve* get() {
		std::lock_guard<std::mutex> lock(*_mtx);
		if(_plot->empty())
			return nullptr;
		auto pl=_plot->front();
		_plot->pop_front();
		return pl;
	}

private:
	bool parse(std::string s)
	{
		std::istringstream is(s);
		double v;
		if(is>>v) {
			x.push_back(v);
			if(y.empty()) {
				for(int i=0; is>>v; ++i) {
					y.push_back(std::vector<double>());
					y[i].push_back(v);
				}
			} else {
				for(int i=0; i < y.size(); ++i) {
					is>>v;
					y[i].push_back(v);
				}
			}
			return 0;

		} else
			return 1;
	}

	void flush_plot()
	{
		static QColor color[]={ QColor("red"), QColor("blue"), QColor("black"), QColor("green")};
		static int idx=0;
		if(x.empty())
			return;
		std::lock_guard<std::mutex> lock(*_mtx);
		for(int i=0; i < y.size(); ++i) {
			auto pl=new QwtPlotCurve("plot");
			pl->setPen(QPen(color[idx++],2));
			idx%=4;
			pl->setTitle(title.c_str());
			pl->setSamples(x.data(), y[i].data(), x.size());
			_plot->push_back(pl);
		}
		x.clear();
		y.clear();
		title.clear();
	}

	std::string title;
	std::vector<double> x;
	std::vector<std::vector<double>> y;

	std::shared_ptr<std::mutex> _mtx;
	std::shared_ptr<std::list<QwtPlotCurve*>> _plot;
	std::vector<std::string> _file;
};


#endif
