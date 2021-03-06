#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h" 
#include "io_cfg.h"         
#include "user\user.h"

#include "user\ds_pwm.h"
#include "user\ds_adc.h"

//-----------------------------
#pragma config CPUDIV = OSC1_PLL2,PLLDIV = 5,USBDIV = 2
#pragma config FOSC = HSPLL_HS,FCMEN = OFF,IESO = OFF
#pragma config PWRT = OFF,BOR = OFF,VREGEN = ON
#pragma config WDT = OFF,MCLRE = ON,LPT1OSC = OFF
#pragma config PBADEN = OFF,LVP = OFF,ICPRT = OFF


static void InitializeSystem(void);
void USBTasks(void);

#define MotorPort  LATD

void InitADC(void);
//void InitPWM(void);

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

    TRISA = 0x00;
    PORTA = 0x00;

    TRISB = 0x00;
    PORTB = 0xFF;

    InitializeSystem();
    setup();

    // PWM
    InitCCP1PWM(255, 0x03);
    SetDutyCycleCCP1(512);

    InitADC();

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

#define IS_ADC_INTERRUPT() PIR1bits.ADIF && PIE1bits.ADIE // ADC interrupt bits set and enable
#define GET_ADC_8_BIT_RESULT() ADRESH;
#define RESET_ADC_INTERRUPT() PIR1bits.ADIF = 0

void StartADC()
{
    ADCON0bits.GO_DONE = 1; // Start adc
    PIR1bits.ADIF = 0; // Reset ADC interrupt
}

void InitADC(void)
{
    ADCON0 = 0b00000001; // A/D enable and select AN0 (0x01 | 0bxx0000xx)
    ADCON1 = 0b00001110; // Inranl voltage and only AN0 to didgital input (0bxx00xx | 0bxxxx1110)
    ADCON2 = 0b00111110; // ACQT = 20TAD (33us/4.1us) and clock FOSC/64 (0bxx111xxx | 0bxxxxx110)

    ADCON2bits.ADFM = 0; // Left justified. Good to use 8 high bits form ADRESH
    PIE1bits.ADIE = 1; // Enable A/D interrupt
    IPR1bits.ADIP = 1; // Set high priority for A/D interrupt

    StartADC();
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
        //PORTB = ~PORTB;
        count = 0;
     }
}

  void HighInterrupt(void)
{
    if (IS_ADC_INTERRUPT())
    {
        RESET_ADC_INTERRUPT();
        PORTB = GET_ADC_8_BIT_RESULT();
        StartADC();
    }
     INTCONbits.TMR0IF = LOW;
     count++;
     if (count > 10) {
        //PORTA = ~PORTA;
        count = 0;
     }
}
