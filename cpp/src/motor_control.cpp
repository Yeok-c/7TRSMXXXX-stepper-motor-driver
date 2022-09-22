#include "TRSM.h"
#include <iostream>  // for std::cout and std::cin

int main(){
    TRSM trsm(1);

    std::cout << "\n Available commands (see numpad layout): \n 8 = open"
    "\n 2 = close\n 6 = open by small increment\n 4 = close by small increment"
    "\n l = explicitly lock when motor is stopped \n u = explicitly unlock when motor is stopped \n f = reflash all parameters";
    
    while(true){
        std::cout << "\nEnter a command: ";
        unsigned char x{ }; // define variable x to hold user input (and zero-initialize it)
        std::cin >> x;
        switch(x){
            case '8':
                trsm.open(1);
            break;
            case '2':
                trsm.close(1);
            break;
            case '6':
                trsm.n_steps(1,-500);
            break;
            case '4':
                trsm.n_steps(1,500);
            break;
            case 'l':
                trsm.config_lock_when_stop(1, true);
                trsm.flash_parameters(1);
                trsm.n_steps(1,1);
            break;
            case 'u':
                trsm.config_lock_when_stop(1, false);
                trsm.flash_parameters(1);
                trsm.n_steps(1,1);
            break;
            case 'f':
                // trsm.flash_general_config(1); THIS IS ONLY CONFIGURED FOR ADDR=1
                trsm.change_accel_param(1, 0.01);
                trsm.change_accel_steps(1, 2000);
                trsm.change_microstep(1,32);
                trsm.change_period_initial(1, 250);
                trsm.change_period_constant(1, 800);
                trsm.config_lock_when_stop(1, true);
                trsm.flash_parameters(1);
                trsm.n_steps(1,1); //stepper needs to activate to save values it seems
            break;
        }

    }
}

