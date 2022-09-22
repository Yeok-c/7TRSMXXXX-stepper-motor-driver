# 7TRSMXXXX-stepper-motor-driver

## Lightweight c++ library for RS485-modbus-rtu communication between 7TRSMXXXX back-mounted stepper motor drivers.
Tested on: Windows 11, 7TRSM5730

motor_control.cpp is the example program for using the functions.
TRSM.cpp/.h is the main bulk of the scripts
serialib.cpp/.h is the serial library used for cross-platform support (linux, windows, mac)

- Not tested on linux yet but the only window-specific dependency is windows.h (in TRSM) for Sleep function
