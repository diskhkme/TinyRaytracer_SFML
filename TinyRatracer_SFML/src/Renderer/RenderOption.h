#pragma once

#define HIGHP

#ifdef HIGHP
typedef double dist_t;
#else
typedef float dist_t;
#endif

// TODO : Add precision option regarding vec3

#define EPS 1e-3



