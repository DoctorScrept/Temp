
//-- I N C L U D E S ------------------------
#include <p18cxxx.h>
#include "system\typedefs.h"
#include "system\usb\usb.h"
#include "io_cfg.h"             // I/O pin mapping
#include "user\user.h"

#define STOP  0
#define LEFT  1
#define RIGHT 2


//-- V A R I A B L E S ----------------------
#pragma udata
const rom byte PATTERN[4] ={0x01,0x04,0x02,0x08};  // 1-Phase drive or full step drive pattern modifidable
static char Direction=0;
static word MotorCount=0;
static char PatPtr=0; 	 
byte counter;
DATA_PACKET dataPacket;
float resultCounter = 0;
float test[2] ={0.0f, 2.0f};

//-- P R I V A T E  P R O T O T Y P E S ------
void ServiceRequests(void);
void UpdateMotor(byte Dir);

#pragma code
//--------------------------------------------
void UpdateMotor(byte Dir)
{ 	      
	 if (Dir!=STOP)
	 {
	 	if (Dir==LEFT)
	 	{
		 	
		 	if (MotorCount==0)
		 	{
			 	MotorCount=1000U;	// Modifidable for speed motor
		 		PatPtr--;
		 		if (PatPtr < 0) PatPtr = 3;	 	 	
		 		MotorPort = PATTERN[PatPtr];  
		 	}
		 	MotorCount--;
	 	}
	 	else
	 	if (Dir==RIGHT)
	 	{
		 	if (MotorCount==0)
		 	{
			 	MotorCount=1000U;	// Modifidable for speed motor
		    	PatPtr++;
		 		if (PatPtr > 3) PatPtr = 0;		 	
		 		MotorPort = PATTERN[PatPtr];
		 	}
		 	MotorCount--;
	 	}		 
	 }	 	 
}	

//----------------------------------
// 
//----------------------------------
void UserInit(void)
{
    mInitMotor();    
}

//----------------------------------
// Process IO
//----------------------------------
void ProcessIO(void)
{   
    if ((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) return;    
    ServiceRequests();  
    UpdateMotor(Direction);
}

//----------------------------------
// ServiceRequests
//-----------------------------------
void ServiceRequests(void)
{
    byte index;
    
    if(USBGenRead((byte*)&dataPacket,sizeof(dataPacket)))  // Read Data Packet from CPU
    {
        counter = 0;
        switch(dataPacket.CMD)
        {
            case DATA_PACKET.READ_VERSION:       // defined in user.h
                 //dataPacket._byte[0] is  READ_VERSION command
                 //dataPacket._byte[1] is len = 2 (MINOR_VERSION & MAJOR_VERSION)
                 dataPacket._byte[2] = MINOR_VERSION;  // defined in user.h
                 dataPacket._byte[3] = MAJOR_VERSION;  // defined in user.h
                 counter=0x04;
                 break;
            //---------------------------    
            case DATA_PACKET.MOTOR_ON:           // defined in user.h
                 Direction = dataPacket._byte[2];   // 0= stop , 1 = turn left, 2 = turn right
                 //dataPacket._byte[0] is MOTOR_ON command
                 //mLED_1_Toggle();
            	 counter=0x1;
            	 break;
            case DATA_PACKET.TEST:           // defined in user.h
                counter=16;//0x8;
                resultCounter += 0.1f;
                dataPacket.floats[0] = test[1];
                dataPacket.floats[1] = test[0];
                break;
            default:
                 break;
        }
        
        if(counter != 0)
        {
            if(!mUSBGenTxIsBusy())
                USBGenWrite((byte*)&dataPacket,counter);
        }
    }

}



