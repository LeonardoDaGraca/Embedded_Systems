#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <pigpio.h>

const int outMotor = 21;

void binary_mode();
void speed_mode();
int handle_binary_input();

void print_menu() {
    printf("\n");
    printf("Welcome to motor module!\n");
    printf("------------------------\n");
    printf("Options\n");
    printf("Enter 1: Binary Mode (Motor on or off)\n");
    printf("Enter 2: Select Motor Speed\n");
    printf("Enter CTRL-C: To exit\n");
}

void binary_mode() {
    printf("\n");
    printf("Binary Mode\n");
    printf("-----------\n");
    printf("Enter 1: Switch Motor on/off\n");
    printf("Enter 2: Return to Main Menu\n");
    printf("Enter CTRL-C: Exit Application\n");
    handle_binary_input();
}

void speed_mode() {
    printf("\n");
    printf("Speed Mode\n");
    printf("-----------\n");
//    printf("Enter 1: Switch Motor on/off\n");
//    printf("Enter 2: Return to Main Menu\n");
    printf("Enter CTRL-C: Exit Application\n");
}

int handle_menu_input() {
    char input[10];
    if (fgets(input, sizeof(input), stdin)) {
        if (input[0] == '1') {
            //run binary mode
            binary_mode();
        } else if (input[0] == '2') {
            //run select mode
            speed_mode();
        } else {
            printf("Enter a valid selection\n");
        }
    }

    return 0; // Continue running
}

int handle_binary_input() {
    char input[10];
    if (fgets(input, sizeof(input), stdin)) {
        if (input[0] == '1') {
            if (gpioRead(outMotor) == PI_LOW) {
                for (int i = 0; i < 256; i++) {
                    gpioPWM(outMotor, i);
                    time_sleep(0.01);
                }
                time_sleep(0.1);
                gpioPWM(outMotor, 255);
            }
            else {
                for (int i = 255; i >= 0; i--) {
                    gpioPWM(outMotor, i);
                    time_sleep(0.01);
                }
                time_sleep(0.1);
                gpioPWM(outMotor, i);
            }
        } else if (input[0] == '2') {
            //return to the menu
            print_menu();
        } else {
            printf("Enter a valid selection");
        }
    }

    return 0; // Continue running
}


#endif