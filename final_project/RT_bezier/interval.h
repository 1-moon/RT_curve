
#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits> // This is necessary for std::numeric_limits
#include <algorithm> // This is necessary for fmin and fmax

class Interval {
public:
    double min, max;	// two data members
    static const Interval empty;
    static const Interval universe;
    

    // constructors
    Interval() : min(+std::numeric_limits<double>::infinity()), max(-std::numeric_limits<double>::infinity()) {}	
    Interval(double _min, double _max) : min(_min), max(_max) {}
    Interval(const Interval& a, const Interval& b)
        : min(fmin(a.min, b.min)), max(fmax(a.max, b.max)) {}

    // methods
    double size() const { return max - min; }
    bool contains(double x) const { return x >= min && x <= max; }
    bool surrounds(double x) const { return min < x && x < max; }

    Interval expand(double delta) const {
        auto padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    friend Interval operator+(const Interval& ival, double displacement);
    friend Interval operator+(double displacement, const Interval& ival);
};


#endif // INTERVAL_H
