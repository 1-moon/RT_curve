
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// usings 
using std::shared_ptr;
using std::make_shared;
using std::numeric_limits;
using std::sqrt;


// constants
const double infinity = numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

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
