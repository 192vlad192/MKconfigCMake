#ifndef __MAIN_C_TEST__
#define __MAIN_C_TEST__

#include "stdint.h"

void init(void);
void initClockingHSE(void);
void initGPIO(void);
void wait(uint32_t val);
int main(void);

#endif //__MAIN_C_TEST__
