/*
	new file where we define infinity 
*/

#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <limits>
#include <memory>

// usings 
using std::shared_ptr;
using std::make_shared;

// constants
const double infinity = std::numeric_limits<double>::infinity();

// Headers
#include "ray.h"
#include "interval.h"

#endif
