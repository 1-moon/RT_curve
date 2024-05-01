// interval.cpp
#include "Interval.h"

const Interval Interval::empty(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
const Interval Interval::universe(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());


Interval operator+(const Interval& ival, double displacement) {
    return Interval(ival.min + displacement, ival.max + displacement);
}

Interval operator+(double displacement, const Interval& ival) {
    return ival + displacement;
}