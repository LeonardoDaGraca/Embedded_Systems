/*
 * Leonardo DaGraca
 * HW3: Controlling Hardware
 * 
 * File contains program that accomplishes Task A
 * of the HW. Upon running the program the red led
 * will flash 3 times followed by the yellow and 
 * then the green. Use CTRL-C to exit.
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

void flash(const int LED, int iterations)
{
	int n;
	for(n = 0; n < iterations; n++) {
		gpioWrite(LED, PI_HIGH);
		time_sleep(1);
		gpioWrite(LED, PI_LOW);
		time_sleep(1);
	}
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
		flash(RedLED, 3);
		flash(YelLED, 3);
		flash(GreLED, 3);
	}

	gpioSetMode(RedLED, PI_INPUT);
	gpioSetMode(YelLED, PI_INPUT);
	gpioSetMode(GreLED, PI_INPUT);
	gpioTerminate();
	printf("\n");

	return 0;	
}
