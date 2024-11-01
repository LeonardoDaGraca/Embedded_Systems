/*
 * Leonardo DaGraca 
 * HW5 : Analog Signals using a DC Motor
 * 
 * Program allows the user to do the following:
 * make motor work in a binary way
 * make motor work at varying speeds
 * 
 * All functionality will signify how fast the motor is moving
 * by using LEDs
 * Red LED = Low or no motor movement
 * Yellow LED = moderate motor movement
 * Green LED = high motor movement
 */
 
#include <stdio.h>
#include <signal.h>
#include <pigpio.h>
#include <stdlib.h>
#include "motor.h"


void sigint_handler(int signal) {
    signal_received = signal;
}


int main() {
    if (gpioInitialise() == PI_INIT_FAILED) {
        printf("ERROR: Failed to initialize the GPIO interface.\n");
    }
    gpioSetMode(OUT_MOTOR, PI_OUTPUT);
    gpioSetMode(OUT_LOW, PI_OUTPUT);
    gpioSetMode(OUT_MODERATE, PI_OUTPUT);
    gpioSetMode(OUT_HIGH, PI_OUTPUT);
    
    print_menu();

    signal(SIGINT, sigint_handler);
    while (!signal_received) {
        handle_menu_input();
    }
    gpioSetMode(OUT_MOTOR, PI_INPUT);
    gpioSetMode(OUT_LOW, PI_INPUT);
    gpioSetMode(OUT_MODERATE, PI_INPUT);
    gpioSetMode(OUT_HIGH, PI_INPUT);
    gpioTerminate();

    printf("\n");

    return 0;
}
