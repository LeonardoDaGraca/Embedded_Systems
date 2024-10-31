#include <stdio.h>
#include <signal.h>
#include <pigpio.h>
#include <stdlib.h>
#include "motor.h"

volatile sig_atomic_t signal_received = 0;
//const int outMotor = 21;

void sigint_handler(int signal) {
    signal_received = signal;
}


int main() {
//    printf("Press Ctrl-C to exit.\n");
    if (gpioInitialise() == PI_INIT_FAILED) {
        printf("ERROR: Failed to initialize the GPIO interface.\n");
    }
    gpioSetMode(outMotor, PI_OUTPUT);
    print_menu();

    //signal(SIGINT, sigint_handler);
    while (!signal_received) {
        handle_menu_input();
    }
    gpioSetMode(outMotor, PI_INPUT);
    gpioTerminate();

    printf("\n");

    return 0;
}