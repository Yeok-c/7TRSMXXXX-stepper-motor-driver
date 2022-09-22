#include "Modbus.h"
#include "string.h"
#include <iostream>

#include "serialib.h"
#include <stdio.h>
#include <windows.h>

#if defined (_WIN32) || defined(_WIN64)
    //for serial ports above "COM9", we must use this extended syntax of "\\.\COMx".
    //also works for COM0 to COM9.
    //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea?redirectedfrom=MSDN#communications-resources
    #define SERIAL_PORT "\\\\.\\COM5"
#endif
#if defined (__linux__) || defined(__APPLE__)
    #define SERIAL_PORT "/dev/ttyACM0"
#endif

// #define ADDR  1
// #define ACCEL_PARAM  0.01
// #define ACCEL_STEPS  2000
// #define MICROSTEP  32
// #define SWITCH_OFFSET  1000
// #define PERIOD_INITIAL  250
// #define PERIOD_CONSTANT  800
// #define FULL_REVOLUTION_STEPS 171072 
// //198*27*32 //360/~1.8 (angle/nema_step) * 27 gear reduction * 32 microsteps 
// #define OPENING_ANGLE 133

class TRSM{
    public:
        explicit TRSM(        
            int ADDR
        );
        ~TRSM();
        
        unsigned char addr;
        float accel_param;
        int accel_steps;
        unsigned short microstep;
        int switch_offset;
        unsigned short period_initial;
        unsigned short period_constant;
        unsigned int full_revolution_steps;
        unsigned int opening_angle;

        unsigned char n_steps(int mDeviceAddr, int steps);
        unsigned char move_angle(int mDeviceAddr, int angle);
        unsigned char open(int mDeviceAddr);
        unsigned char close(int mDeviceAddr);
        unsigned char change_addr(int mDeviceAddr, unsigned char mRegData);
        unsigned char change_accel_param(int mDeviceAddr, float mRegDataFloat);
        unsigned char change_accel_steps(int mDeviceAddr, int mRegData32);
        unsigned char change_microstep(int mDeviceAddr, unsigned short mRegData);
        // unsigned char change_switch_offset(int mDeviceAddr, int mRegData32);
        unsigned char change_period_initial(int mDeviceAddr, unsigned short mRegData);
        unsigned char change_period_constant(int mDeviceAddr, unsigned short mRegData);
        unsigned char config_lock_when_stop(int mDeviceAddr, bool state);
        unsigned char flash_general_config(int mDeviceAddr);
        unsigned char flash_parameters(int mDeviceAddr);
        serialib serial;

    private:
        unsigned char SendFunction16(
            unsigned char mDeviceAddr,
            unsigned short mRegStartAddr,
            unsigned short mRegNumber,
            CModBusCmd16 *pModBusCmd16,
            unsigned int mResTimeOut 
            );

        unsigned char SendFunction06(
            unsigned char mDeviceAddr,
            unsigned short mRegStartAddr,
            unsigned short mRegData,
            unsigned int mResTimeOut );

        void append(char* s, char c);
};