
#if defined (_WIN32) || defined(_WIN64)
    //for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
    //also works for COM0 to COM9.
    //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN#communications-resources
    #define SERIAL_PORT "\\\\.\\COM5"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyACM0"
#endif


// Serial library
#include "serialib.h"
#include <stdio.h>
#include <windows.h>

/*!
 * \brief main  Simple example that send ASCII characters to the serial device
 * \return      0 : success
 *              <0 : an error occured
 */
int main( /*int argc, char *argv[]*/)
{
    // Serial object
    serialib serial;
    // Connection to serial port
    char errorOpening = serial.openDevice(
        SERIAL_PORT, 
        9600,
        SERIAL_DATABITS_8,
        SERIAL_PARITY_NONE,
        SERIAL_STOPBITS_1
        );

        // ser.timeout = 0.5  # non-block read 0.5s
        // ser.writeTimeout = 0.5  # timeout for write 0.5s
        // ser.xonxoff = False  # disable software flow control
        // ser.rtscts = False  # disable hardware (RTS/CTS) flow control
        // ser.dsrdtr = False  # disable hardware (DSR/DTR) flow control


    // If connection fails, return the error code otherwise, display a success message
    if (errorOpening!=1){
        return errorOpening;
    }
    printf ("Successful connection to %s\n",SERIAL_PORT);


    // Display ASCII characters (from 32 to 128)
    unsigned char output[] = {
        0x01, 0x10, 0x07, 0xd6, 0x00, 0x02, 0x04, 0x00, 0x00, 0x03, 0xe8, 0x58, 0x57
        };

    for (int i=0;i<13;i++)
    {
        //Obviously wrong because it's single char (not hex)
        //Need to 1. Group chars into two, 2. Convert char into hex and send
        //Or get from the other program directly
        serial.writeChar(output[i]);
        printf("%02X", output[i]);
    }

    // Close the serial device
    serial.closeDevice();

    return 0 ;
}

