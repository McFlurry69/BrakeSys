#ifndef INCLUDE_TF_CALCULATION_H_
#define INCLUDE_TF_CALCULATION_H_

#include "stdlib.h"
//#include "fuzzy.h"
#include "math.h"

#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define DC_MIN_PWM 0
#define DC_MAX_PWM 10000

void selfStabilizing(void);

#endif
