/*
 * Leonardo DaGraca 
 * HW5 : Analog Signals using a DC Motor
 * 
 * Header file with utility functions to assist motor.c
 */
 
#ifndef MOTOR_H
#define MOTOR_H

#include <stdio.h>
#include <pigpio.h>

const int OUT_MOTOR = 21;
const int OUT_LOW = 13;
const int OUT_MODERATE = 19;
const int OUT_HIGH = 26;

volatile sig_atomic_t signal_received = 0;

void binary_mode();
void speed_mode();
int handle_binary_input();
void toggle_motor();
void set_speed(int desired_speed);
int led_out(int state);
void parse_speed(int speed);

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
    while (!signal_received) {
        printf("\n");
        printf("Binary Mode\n");
        printf("-----------\n");
        printf("Enter 1: Switch Motor on/off\n");
        printf("Enter 2: Return to Main Menu\n");
        printf("Enter CTRL-C: Exit Application\n");
        
	char input[10];
	if (fgets(input, sizeof(input), stdin)) {
	    if (signal_received) return;

	    if (input[0] == '1') {
	        toggle_motor();
	    } else if (input[0] == '2') {
                print_menu();
		break;
            } else {
	        printf("Enter a valid selection\n");
	    }
	}
    }
}

void speed_mode() {
    while (!signal_received) {
	printf("\n");
	printf("Select Speed Mode\n");
	printf("-----------\n");
	printf("Enter s <speed>: Set Motor Speed (0-255)\n");
	printf("Enter 2: Return to Main Menu\n");
	printf("Enter CTRL-C: Exit Application\n");
	
	char input[10];
	if (fgets(input, sizeof(input), stdin)) {
	    if (signal_received) return;

	    if (input[0] == 's') {
	        int desired_speed;
		if (sscanf(input + 1, "%d", &desired_speed) == 1) {
		    set_speed(desired_speed);
		}
		else {
		    printf("Invalid speed input. Please enter an integer between 0 - 255\n");
		}
	    } else if (input[0] == '2') {
                print_menu();
		break;
            } else {
	        printf("Enter a valid selection\n");
	    }
	}
    }
}

int handle_menu_input() {
    char input[10];
    if (fgets(input, sizeof(input), stdin)) {
	if (signal_received) return 1;

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

    return 0;
}

void toggle_motor() {
    if (gpioRead(OUT_MOTOR) == PI_LOW) { 
        //motor on 
        int i; 
        for (i = 0; i < 256; i++) {  
            gpioPWM(OUT_MOTOR, i); 
            time_sleep(0.01); 
        } 
            time_sleep(0.1); 
            gpioPWM(OUT_MOTOR, 255); 
	    parse_speed(255);
            printf("Motor is ON\n"); 
    } 
    else { 
	//motor off
        int i; 
        for (i = 255; i >= 0; i--) { 
            gpioPWM(OUT_MOTOR, i); 
            time_sleep(0.01); 
        } 
        time_sleep(0.1); 
        gpioPWM(OUT_MOTOR, 0);
	parse_speed(0);
	printf("Motor is OFF\n"); 
    }
}

void set_speed(int desired_speed) {
    //clamp speeds
    if (desired_speed < 0) desired_speed = 0;
    if (desired_speed > 255) desired_speed = 255;
    
    gpioWrite(OUT_MOTOR, PI_LOW);
    int i;
    for (i = 0; i <= desired_speed; i++) {
	gpioPWM(OUT_MOTOR, i);
	time_sleep(0.01);
    }
    time_sleep(0.1);
    gpioPWM(OUT_MOTOR, desired_speed);
    parse_speed(desired_speed);
    
    int percentage = (desired_speed * 100) / 255;
    printf("Motor is running at %d%% speed\n", percentage);
}

int led_out(int state) {
    gpioWrite(OUT_LOW, PI_LOW);
    gpioWrite(OUT_MODERATE, PI_LOW);
    gpioWrite(OUT_HIGH, PI_LOW);
    
    if (state == 0) {
	//turn red led on
	gpioWrite(OUT_LOW, PI_HIGH);
	return 0;
    }
    if (state == 1) {
	//turn yellow led on
	gpioWrite(OUT_MODERATE, PI_HIGH);
	return 0;
    }
    if (state == 2) {
	//turn green led on
	gpioWrite(OUT_HIGH, PI_HIGH);
	return 0;
    }
    return 1;
}

void parse_speed(int speed) {
    if (speed >= 0 && speed <= 65) {
	led_out(0);
    }
    else if (speed > 65 && speed <= 165) {
	led_out(1);
    }
    else if (speed > 165 && speed <= 255){
	led_out(2);
    }
}

#endif
