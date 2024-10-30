#include <signal.h>
#include <stdio.h>
#include <pigpio.h>

const int outMotor = 21;

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
	gpioSetMode(outMotor, PI_OUTPUT);
	signal(SIGINT, sigint_handler);
	printf("Press Ctrl-C to exit.\n");
	
	while (!signal_received) {
		for (int i = 0; i < 256; i++) {
			gpioPWM(outMotor, i);
			time_sleep(0.01);		
		}
		time_sleep(0.1);
	}

	gpioSetMode(outMotor, PI_INPUT);
	gpioTerminate();
	printf("\n");

	return 0;	
}

