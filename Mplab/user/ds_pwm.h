#ifndef DS_PWM_H
#define	DS_PWM_H

#include <p18cxxx.h>

void InitCCP1PWM(char pr2, char prescaler);
void SetDutyCycleCCP1(int value);
int GetDutyCycleCCP1();

#endif	/* DS_PWM_H */

