
#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// usings 
using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// functiosn 
inline double random_double() {
	return rand() / (RAND_MAX + 1.0);	// returns real in [0,1). 
}
inline double random_double(double min, double max) {
	return min + (max - min) * random_double();	// returns real in [min,max).
}

inline int random_int(int min, int max) {
	// Returns a random integer in [min,max].
	return static_cast<int>(random_double(min, max + 1));
}
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

// Headers
#include "ray.h"
#include "interval.h"
#include "vec3.h"

#endif
