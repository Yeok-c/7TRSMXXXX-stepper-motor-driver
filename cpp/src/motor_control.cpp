unsigned char addr = 1;
float accel_param = 0.01;
int accel_steps = 2000;
unsigned short microstep = 32;
int switch_offset = 1000;
unsigned short period_initial = 250;
unsigned short period_constant = 800;
#include "TRSM.h"

int main(){
    TRSM trsm;
	trsm.change_addr(addr,1);
	trsm.change_accel_param(addr, accel_param);
	trsm.change_accel_steps(addr, accel_steps);
	trsm.change_microstep(addr,microstep);
	// change_switch_offset(addr, switch_offset);
	trsm.change_period_initial(addr, period_initial);
	trsm.change_period_constant(addr, period_constant);
	trsm.flash_parameters(addr);
}
