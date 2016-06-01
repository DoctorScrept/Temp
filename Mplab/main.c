#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h" 
#include "io_cfg.h"         
#include "user\user.h"    
//-----------------------------
#pragma config CPUDIV = OSC1_PLL2,PLLDIV = 5,USBDIV = 2
#pragma config FOSC = HSPLL_HS,FCMEN = OFF,IESO = OFF
#pragma config PWRT = OFF,BOR = OFF,VREGEN = ON
#pragma config WDT = OFF,MCLRE = ON,LPT1OSC = OFF
#pragma config PBADEN = OFF,LVP = OFF,ICPRT = OFF


static void InitializeSystem(void);
void USBTasks(void);


#define MotorPort  LATD

//-----------------------------------
// main loop 
//-----------------------------------
#pragma code
void main(void)
{   
    InitializeSystem();
    while(1)
    {
        USBTasks();         // USB Tasks
        ProcessIO();        // User Tasks
    }
}

//-----------------------------------
// Initial USB module & User
//-----------------------------------
static void InitializeSystem(void)
{
    ADCON1 |= 0x0F;                 // Default all pins to digital        
    mInitializeUSBDriver();         // See usbdrv.h    
    UserInit();                     // See user.c & .h
}

//-----------------------------------
// USB tasks
//-----------------------------------
void USBTasks(void)
{
    USBCheckBusStatus();                    // Must use polling method
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method

}


