// interval.h


#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits> // This is necessary for std::numeric_limits

class Interval {
public:
    double min, max;	// two data members
    static const Interval empty;
    static const Interval universe;
    

    // constructors
    Interval() : min(+std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}	
    Interval(double _min, double _max) : min(_min), max(_max) {}
    // methods
    double size() const { return max - min; }
    bool contains(double x) const { return x >= min && x <= max; }
    bool surrounds(double x) const { return min < x && x < max; }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }
  
};

#endif // INTERVAL_H
