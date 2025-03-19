#include <assert.h>

// TODO: Test this especially on STM32 cm3 library maybe not work well
#ifdef STM32
#define alb_assert(expr) cm3_assert(expr)
#else
#define alb_assert(expr) assert(expr)
#endif

#define alb_static_assert(expr, msg) static_assert(expr, msg)