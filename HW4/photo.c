/*
 * Leonardo DaGraca
 * HW4: External Signaling via Photocell
 * 
 * Task B: Converted the Python code to correctly operate
 * the photocell configuration in C.
 *
 */

#include <signal.h>
#include <stdio.h>
#include <pigpio.h>
#include <unistd.h> 

//settings
const int IN_RC = 17;
const int OUT_LOW = 16;
const int OUT_HIGH = 20;
const int OUT_STATE = 21;

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal)
{
	signal_received = signal;
}

int RCtime(const int RCpin) {
	int reading = 0;
	gpioSetMode(RCpin, PI_OUTPUT);
	gpioWrite(RCpin, PI_LOW);
	usleep(100000);
	
	gpioSetMode(RCpin, PI_INPUT);
	
	while (gpioRead(RCpin) == PI_LOW) {
		reading++;
		usleep(1000);
	}
	return reading;
}

int ledOut(int state) {
	if (state == 0) {
		gpioWrite(OUT_LOW, PI_HIGH);
		gpioWrite(OUT_HIGH, PI_HIGH);
		return 0;
	}
	if (state == 1) {
		gpioWrite(OUT_LOW, PI_LOW);
		gpioWrite(OUT_HIGH, PI_HIGH);
		return 0;
	}
	if (state == 2) {
		gpioWrite(OUT_LOW, PI_HIGH);
		gpioWrite(OUT_HIGH, PI_LOW);
		return 0;
	}
	if (state == 3) {
		gpioWrite(OUT_LOW, PI_LOW);
		gpioWrite(OUT_HIGH, PI_LOW);
		return 0;
	}
	return 1;
}

char* photocellParse(int reading) {
	char* out;
	if (reading <= 15) {
		out = "0";
		ledOut(0);
		return out;
	}
	if (reading <= 30) {
		out = "o";
		ledOut(1);
		return out;
	}
	if (reading <= 250) {
		out = ".";
		ledOut(2);
		return out;
	}
	out = " ";
	ledOut(3);
	return out;
}

		
int main()
{
	if (gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initialize the GPIO interface.\n");
	}
	
	//init pins
	gpioSetMode(27, PI_OUTPUT);
	gpioWrite(27, PI_HIGH);
	gpioSetMode(OUT_LOW, PI_OUTPUT);
	gpioSetMode(OUT_HIGH, PI_OUTPUT);
	gpioSetMode(OUT_STATE, PI_OUTPUT);
	signal(SIGINT, sigint_handler);
	printf("Press Ctrl-C to exit.\n");
	
	while (!signal_received) {
		gpioWrite(OUT_STATE, PI_HIGH);
		printf("%s", photocellParse(RCtime(IN_RC)));
		fflush(stdout);
		gpioWrite(OUT_STATE, PI_LOW);
	}

	gpioSetMode(OUT_STATE, PI_INPUT);
	gpioSetMode(OUT_HIGH, PI_INPUT);
	gpioSetMode(OUT_LOW, PI_INPUT);
	gpioSetMode(27, PI_INPUT);
	gpioTerminate();
	printf("\n");

	return 0;	
}
