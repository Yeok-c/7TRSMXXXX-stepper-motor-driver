/*
ModBus.c
*/
#include "Modbus.h"


unsigned short ModRtuCrc(unsigned char buf[], int len)
{
  unsigned short crc = 0xFFFF;
	int pos,i;
 
  for (pos = 0; pos < len; pos++) 
	{
    crc ^=buf[pos];          // XOR byte into least sig. byte of crc 
    for (i = 8; i != 0; i--) 
		{    // Loop over each bit
        if ((crc & 0x0001) != 0) 
			  {      // If the LSB is set
           crc >>= 1;                    // Shift right and XOR 0xA001
           crc ^= 0xA001;
        }
        else                            // Else LSB is not set
           crc >>= 1;                    // Just shift right
    }//for (int i = 8; i != 0; i--) 
  }//for (int pos = 0; pos < len; pos++) 
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}


//$
