// interval.h
#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits> // This is necessary for std::numeric_limits

class interval {
public:
    static const interval empty;
    static const interval universe;

    interval() : min(+std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}	// default interval is empty
    interval(double _min, double _max) : min(_min), max(_max) {}

    double size() const { return max - min; }
    bool contains(double x) const { return x >= min && x <= max; }
    bool surrounds(double x) const { return min < x && x < max; }

    double min, max;	// two data members
};

#endif // INTERVAL_H
