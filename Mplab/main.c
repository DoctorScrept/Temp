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

void InitPWM(void);

#define HIGH 1
#define LOW 0
void setup(void);
void LowInterrupt(void);
void HighInterrupt(void);

#pragma code HIGH_INTERRUPT_VECTOR = 0x08
void high_interrupt( void )
{
  _asm goto HighInterrupt _endasm
}
#pragma interrupt HighInterrupt

#pragma code LOW_INTERRUPT_VECTOR = 0x18
void low_interrupt (void)
{
    _asm GOTO LowInterrupt _endasm
}
#pragma interrupt LowInterrupt

//-----------------------------------
// main loop 
//-----------------------------------
#pragma code
int count = 0;

void main(void)
{
   
//    INTCONbits.GIE = 1; //enable interrupts
//RCONbits.IPEN = 1; // turn priority levels on
//INTCON2bits.TMR0IP = 0; //set timer0 to low priority

    TRISA = 0x00;
    PORTA = 0x00;

    TRISB = 0x00;
    PORTB = 0x01;

    InitializeSystem();
    setup();

     InitPWM();

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

int pwm_reg = 212;
char pwm_reg2;

void InitPWM(void)
{
    TRISC = TRISC & 0b11111001;

    CCP1CON=0b00001100; // ????????? ?????? ? ?????? ???
    T2CON=0b01111110; // ????????? ???????
    PR2=55; // ??????? ??????? ???
    CCPR1L=0; // ??????? ???????? ???????????? = 0


//    int pwm_reg;// = 255; // ???????? ??????? ??? ???????????? ????????????
//    char pwm_reg2;// = 0;  // ?????????????? ??????? ??? ?????????
//pwm_reg = 255;
//
 pwm_reg2=pwm_reg<<6;    //
 pwm_reg2=pwm_reg2>>2;   //
 CCP1CON &= 0b11001111;  //
 CCP1CON |= pwm_reg2;    //
 CCPR1L=pwm_reg>>2;      //
}




 void setup()
 {

  RCONbits.IPEN = 1; //enable priority levels
  //T0CON = 0x80; // TMR0 on, prescaler 1/2 16-bit mode
  // 48 MHZ / 4 = 12 MHZ Prescale / 2 = 6 MHZ
  // 1 / 6 MHZ = 1.67 e-7 * 2^16 = .011 Sec.
    T0CONbits.T0PS0=1; //Prescaler is divide by 256
    T0CONbits.T0PS1=1;
    T0CONbits.T0PS2=1;
    T0CONbits.PSA=0;      //Timer Clock Source is from Prescaler
    T0CONbits.T0CS=0;     //Prescaler gets clock from FCPU (5MHz)
    T0CONbits.T08BIT=1;   //8 BIT MODE
    T0CONbits.TMR0ON = 1;


  INTCONbits.TMR0IE = HIGH; // enable TMR0 interrupt
  INTCONbits.TMR0IF = LOW;  // Clear TMR0 (must be after every interrupt)

  // Global enable
  INTCONbits.PEIE = HIGH;   // PERIPHERAL INTERRUPT ENABLE REGISTER 1 bit
  INTCONbits.GIE = HIGH;    // enable global interrupts


  TMR0L = 0;
  TMR0H = 0;





  PIE1bits.TMR1IE = 1;  // enable TMR1 interrupt
  IPR1bits.TMR1IP = 0;  // TMR1 priority level 1 - high, 0 - low
  T1CONbits.TMR1CS  = 0;  // Internal clock - (FOSC/4)
T1CONbits.T1CKPS0 = 1;  // ???????????? 1:8
T1CONbits.T1CKPS1 = 1;  // ....... ticks per second
//  T1CONbits.T1CKPS = 0b11;  // prescaler 1:8

  // TMR1 counts from TMR1 (TMR1L and TMR1H) register value to 0xFFFF
  TMR1L = 0xF0;
  TMR1H = 0xFF;

  PIR1bits.TMR1IF   = 0; // ??????? ???? ?????????? (????? ????? ?? ????????? ? ??????????)
  T1CONbits.TMR1ON  = 1;  // ?????? ???????
}



  void LowInterrupt(void)
{
     PIR1bits.TMR1IF = 0;
     count++;
     if (count > 10) {
        PORTB = ~PORTB;
        count = 0;
     }
}

  void HighInterrupt(void)
{
     INTCONbits.TMR0IF = LOW;
     count++;
     if (count > 10) {
        PORTA = ~PORTA;
        count = 0;
     }
}
