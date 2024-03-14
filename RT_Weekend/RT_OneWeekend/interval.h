#ifndef INTERVAl_H
#define INTERVAl_H

class interval {

	public:
		interval() : min(+infinity), max(-infinity) {}	// default interval is empty 
		
		interval(double _min, double _max) : min(_min), max(_max) {}

		bool contains(double x) const {
			return x >= min && x <= max;
		}

		bool surrounds(double x) const {
			return min < x && x < max;
		}
	public:
		double min, max;	// two data members 

		static const interval empty, universe;
};

const static interval empty		(+infinity, -infinity);
const static interval universe	(-infinity, +infinity);
#endif 
