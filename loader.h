#ifndef DTV_LOADER_H
#define DTV_LOADER_H
#include <unistd.h>
#include <thread>
#include <mutex>
#include <iostream>
#include <sstream>
#include <fstream>



class clgen
{
	std::vector<QBrush> p1, p2;
	size_t i;

public:
	clgen(std::string s)
	: p1{split(s)}
	, p2{split(s)}
	, i{0}
	{
		if(p2.empty()) { p2=p1; p1.clear(); }
	}

	operator QBrush()
	{
		if(!p1.empty()) {
			if(i < p1.size()) return p1[i++];
			else { i=0; p1.clear(); }
		}
		if(p2.empty()) return QBrush("red");
		return p2[i++ % p2.size()];
	}

private:
	static std::vector<QBrush> split(std::string& s)
	{
		std::vector<QBrush> r;
		while(1) {
			auto n=s.find_first_of(", ()[]");
			auto color=s.substr(0,n);
			s.erase(0,n);
			if(!color.empty()) r.push_back(QBrush(color.c_str()));

			if(s.empty()) return r;
			char ch=s[0]; s.erase(0,1);
			if(ch == '(' || ch == ')' 
				|| ch == '[' || ch == ']')
				return r;
		}
	}
};


class loader
{
public:
	loader(std::vector<std::string> file, std::string colors)
	: _mtx{std::make_shared<std::mutex>()}
	, _plot{std::make_shared<std::list<QwtPlotCurve*>>()}
	, _file(file)
	, _palette(colors)
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
		if(x.empty())
			return;
		std::lock_guard<std::mutex> lock(*_mtx);
		for(int i=0; i < y.size(); ++i) {
			auto pl=new QwtPlotCurve("plot");
			pl->setPen(QPen(_palette,2));
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
	clgen _palette;
};


#endif
