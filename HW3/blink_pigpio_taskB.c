/*
 * Leonardo DaGraca
 * HW3: Controlling Hardware
 *
 * File contains program that accomplishes Task B
 * of the HW. Upon running the program the leds 
 * will simulate a traffic light software. The red 
 * led will stay on for 20 secs, the green for 18 secs,
 * and the yellow for 2 secs. 
 * Use CTRL-C to exit.
 */

#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int RedLED = 21;
const int YelLED = 20;
const int GreLED = 16;

volatile sig_atomic_t signal_received = 0;

void sigint_handler(int signal)
{
	signal_received = signal;
}

int main()
{
	if (gpioInitialise() == PI_INIT_FAILED) {
		printf("ERROR: Failed to initialize the GPIO interface.\n");
	}
	gpioSetMode(RedLED, PI_OUTPUT);
	gpioSetMode(YelLED, PI_OUTPUT);
	gpioSetMode(GreLED, PI_OUTPUT);
	signal(SIGINT, sigint_handler);
	printf("Press Ctrl-C to exit.\n");
	
	while (!signal_received) {
		gpioWrite(RedLED, PI_HIGH);
		time_sleep(20);
		gpioWrite(RedLED, PI_LOW);
		gpioWrite(GreLED, PI_HIGH);
		time_sleep(18);
		gpioWrite(GreLED, PI_LOW);
		gpioWrite(YelLED, PI_HIGH);
		time_sleep(2);
		gpioWrite(YelLED, PI_LOW);
	}

	gpioSetMode(RedLED, PI_INPUT);
	gpioSetMode(YelLED, PI_INPUT);
	gpioSetMode(GreLED, PI_INPUT);
	gpioTerminate();
	printf("\n");

	return 0;	
}
