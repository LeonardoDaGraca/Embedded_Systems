/*
 * Leonardo DaGraca
 * HW8 : Thermistor Data Collection
 *
 * This program uses a thermistor to display the temperature of
 * the room in Celsius. It allows the user to set a lower and upper
 * desired temperatures using the respective potentiometer for the
 * action. Based on the desired range assigned by the user, the correct
 * LED will illuminate based on whether it is below the range(Blue),
 * above the range(Red), and within the range(Green).
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <signal.h>

#define POT1_CHANNEL 1 //lower threshold
#define POT2_CHANNEL 2 //upper threshold
#define RED_LED 21
#define GREEN_LED 20
#define BLUE_LED 16

volatile sig_atomic_t signal_received = 0;

/*
 * helper function to read spi handle based on the given channel
 */
int read_adc(int spi_handle, int channel) {
	unsigned char buf[3];
	buf[0] = 1;
	buf[1] = (8 + channel) << 4;
	buf[2] = 0;
	spiXfer(spi_handle, buf, buf, 3);
	return ((buf[1] & 3) << 8) | buf[2];
}

/*
 * helper function to read the threshold reading and map it to a Celsius value
 */
int read_threshold(int channel) {
	int value = read_adc(0, channel);
	return (int)(value * (100.0 / 1023.0)); //maps thresh reading from 0 - 100
}

/*
 * Illuminate the correct LED based on the current temp and the lower and upper bounds
 */
void update_leds(float temp, int lower, int upper) {
	if (temp < lower) {
		gpioWrite(BLUE_LED, PI_HIGH);
		gpioWrite(GREEN_LED, PI_LOW);
		gpioWrite(RED_LED, PI_LOW);
	}
	else if (temp > upper) {
		gpioWrite(BLUE_LED, PI_LOW);
		gpioWrite(GREEN_LED, PI_LOW);
		gpioWrite(RED_LED, PI_HIGH);
	}
	else {
		gpioWrite(BLUE_LED, PI_LOW);
		gpioWrite(GREEN_LED, PI_HIGH);
		gpioWrite(RED_LED, PI_LOW);
	}
}

void sigint_handler(int signal) {
	signal_received = signal;
}

int main(int argc, char *argv[])
{
	int v;
	int spi_handle;

	if(gpioInitialise() < 0) return 1;

	spi_handle = spiOpen(0, 1000000, 0);
	if (spi_handle < 0) return 2;
	gpioSetMode(RED_LED, PI_OUTPUT);
	gpioSetMode(GREEN_LED, PI_OUTPUT);
	gpioSetMode(BLUE_LED, PI_OUTPUT);

	signal(SIGINT, sigint_handler);
	while (!signal_received) {
		v = read_adc(spi_handle, 0); //thermistor
		int lower_threshold = read_threshold(POT1_CHANNEL);
		int upper_threshold = read_threshold(POT2_CHANNEL);
		
		double millivolts = v * (3.3 * 100.0 / 1023.0);
	        double kelvin = millivolts;
		double celsius = millivolts - 273.15;
		double fahrenheight = celsius * 9.0 / 5.0 + 32.0;
		
		update_leds(celsius, lower_threshold, upper_threshold);

		printf("Temp: %.2fC, Lower: %dC, Upper: %dC\n", celsius, 
			lower_threshold, upper_threshold);
		time_sleep(3);
	}

	spiClose(spi_handle);
	gpioSetMode(RED_LED, PI_INPUT);
	gpioSetMode(GREEN_LED, PI_INPUT);
	gpioSetMode(BLUE_LED, PI_INPUT);
	gpioTerminate();
	printf("\n");	

	return 0;
}



