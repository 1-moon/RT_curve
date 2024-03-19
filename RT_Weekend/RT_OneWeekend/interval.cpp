// interval.cpp
#include "interval.h"

const interval interval::empty(+std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity());
const interval interval::universe(-std::numeric_limits<double>::infinity(), +std::numeric_limits<double>::infinity());
