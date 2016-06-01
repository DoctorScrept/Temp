
#ifndef IO_CFG_H
#define IO_CFG_H

#include "autofiles\usbcfg.h"
#define INPUT_PIN           1
#define OUTPUT_PIN          0
#define usb_bus_sense       1
#define self_power          1
//-----------------------------------
#define mInitMotor() LATD &= 0x00; TRISD &= 0x00;
#define MotorPort   		LATD


#endif //IO_CFG_H
