#include "user\ds_pwm.h"


void SetDutyCycleCCP1(int value)
{
    CCP1CON &= 0b11001111; // clear low bits
    CCP1CON |= (value << 4) & 0b00110000; // set low bits
    CCPR1L = value >> 2; // set high bits
}

int GetDutyCycleCCP1()
{
    return ((CCPR1L << 2) | (CCP1CON >> 4 & 0b00000011)) & 0x03FF;
}

void InitCCP1PWM(char pr2, char prescaler)
{
    TRISC = TRISC & 0b11111011; // CCP1 pin to out

    CCP1CON = 0b00001100; //Set to PWM mode (0bxxxx11xx)
    T2CON = 0b00000100 | (prescaler & 0b00000011); // timer on and prescaler 2 bits
    PR2 = pr2;
}