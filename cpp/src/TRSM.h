#include "Modbus.h"
#include "string.h"
#include <iostream>

#include "serialib.h"
#include <stdio.h>
#include <windows.h>

#define ADDR  1
#define ACCEL_PARAM  0.01
#define ACCEL_STEPS  2000
#define MICROSTEP  32
#define SWITCH_OFFSET  1000
#define PERIOD_INITIAL  250
#define PERIOD_CONSTANT  800

class TRSM{
    public:
        unsigned char addr;
        float accel_param;
        int accel_steps;
        unsigned short microstep;
        int switch_offset;
        unsigned short period_initial;
        unsigned short period_constant;

        unsigned char n_steps(int mDeviceAddr, long steps);
        unsigned char change_addr(int mDeviceAddr, unsigned char mRegData);
        unsigned char change_accel_param(int mDeviceAddr, float mRegDataFloat);
        unsigned char change_accel_steps(int mDeviceAddr, int mRegData32);
        unsigned char change_microstep(int mDeviceAddr, unsigned short mRegData);
        // unsigned char change_switch_offset(int mDeviceAddr, int mRegData32);
        unsigned char change_period_initial(int mDeviceAddr, unsigned short mRegData);
        unsigned char change_period_constant(int mDeviceAddr, unsigned short mRegData);
        unsigned char flash_parameters(int mDeviceAddr);

        TRSM(        
            unsigned char addr, // address
            float accel_param = ACCEL_PARAM,
            int accel_steps = ACCEL_STEPS,
            unsigned short microstep = MICROSTEP,
            int switch_offset = SWITCH_OFFSET,
            unsigned short period_initial = PERIOD_INITIAL,
            unsigned short period_constant = PERIOD_CONSTANT
        ){
            // change_addr(addr,1);
            change_accel_param(addr, accel_param);
            change_accel_steps(addr, accel_steps);
            change_microstep(addr,microstep);
            // change_switch_offset(addr, switch_offset);
            change_period_initial(addr, period_initial);
            change_period_constant(addr, period_constant);
            flash_parameters(addr);
        }



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