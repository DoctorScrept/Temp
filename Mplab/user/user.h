
#ifndef PICDEM_FS_DEMO_H
#define PICDEM_FS_DEMO_H

/** I N C L U D E S **********************************************************/
#include "system\typedefs.h"

/** D E F I N I T I O N S ****************************************************/
/* PICDEM FS USB Demo Version */
#define MINOR_VERSION   0x01  
#define MAJOR_VERSION   0x02


/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    byte _byte[USBGEN_EP_SIZE];  //For byte access
    word _word[USBGEN_EP_SIZE/2];//For word access(USBGEN_EP_SIZE msut be even)
    float floats[USBGEN_EP_SIZE/8];
    struct
    {
        enum
        {
            READ_VERSION    = 0x00,       
            MOTOR_ON		= 0x01,  
			TEST			= 0x02,  
            RESET           = 0xFF
        }CMD;
        byte len;
    };
    struct
    {
        unsigned :8;
        byte ID;
    };
    struct
    {
        unsigned :8;
        byte led_num;
        byte led_status;
    };
    struct
    {
        unsigned :8;
        word word_data;
    };
    struct
    {
        unsigned :8;
        byte read_number;
    };
    
} DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/
void UserInit(void);
void ProcessIO(void);

#endif //PICDEM_FS_DEMO_H
