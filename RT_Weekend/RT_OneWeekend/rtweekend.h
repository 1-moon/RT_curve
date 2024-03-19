
#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <memory>

// usings 
using std::shared_ptr;
using std::make_shared;

// constants
const double infinity = std::numeric_limits<double>::infinity();

// Utility Functions
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}


// Headers
#include "ray.h"
#include "interval.h"
#include "vec3.h"

#endif
