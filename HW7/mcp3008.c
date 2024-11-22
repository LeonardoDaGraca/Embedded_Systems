/*
 * Leonardo DaGraca 
 * HW7 : A2D Conversion
 * 
 * Program allows the user to do the following:
 * control brightness of LED with one potentiometer
 * control flash speed of LED with another potentiometer
 * 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>
#include <pthread.h>

#define RED_LED 21
#define POT1_CHANNEL 0 //brightness channel
#define POT2_CHANNEL 1 //flash channel

/*
 gcc -pthread -o mcp3008 mcp3008.c -lpigpio -lrt
 mcp3008 [num_of_loops] [speed_of_transmission]
 defaults to 1000000 in both cases
*/
volatile int flash_delay = 500;
pthread_mutex_t delay_mutex;

void *flash_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&delay_mutex);
        int delay_ms = flash_delay;
        pthread_mutex_unlock(&delay_mutex);

        gpioWrite(RED_LED, PI_HIGH);
        time_sleep(delay_ms / 1000.0);
        gpioWrite(RED_LED, PI_LOW);
        time_sleep(delay_ms / 1000.0);
    }
    return NULL;
}

int read_adc(int spi_handle, int channel) {
    unsigned char buf[3];
    buf[0] = 1;
    buf[1] = (8 + channel) << 4;
    buf[2] = 0;
    spiXfer(spi_handle, buf, buf, 3);
    return ((buf[1] & 3) << 8) | buf[2];
}

int main(int argc, char *argv[]) {
    int loops;
    int speed;
    int spi_handle;

    if (argc > 1) loops = atoi(argv[1]);
    else loops = 1000000;

    if (argc > 2) speed = atoi(argv[2]);
    else speed = 1000000;

    if (gpioInitialise() < 0) return 1;
    gpioSetMode(RED_LED, PI_OUTPUT);

    spi_handle = spiOpen(0, speed, 0);
    if (spi_handle < 0) return 2;

    pthread_t flash_tid;
    pthread_mutex_init(&delay_mutex, NULL);
    pthread_create(&flash_tid, NULL, flash_thread, NULL);

    for (int i = 0; i < loops; i++) {
       int brightness = read_adc(spi_handle, POT1_CHANNEL);
       int delay_value = read_adc(spi_handle, POT2_CHANNEL);

       int pwm = (int)((1023 - brightness) / 1024.0 * 256.0);
       gpioPWM(RED_LED, pwm);

       pthread_mutex_lock(&delay_mutex);
       flash_delay = (int)((delay_value / 1024.0) * 1000);
       pthread_mutex_unlock(&delay_mutex);

       time_sleep(0.1);
    }
    pthread_cancel(flash_tid);
    pthread_join(flash_tid, NULL);

    pthread_mutex_destroy(&delay_mutex);
    spiClose(spi_handle);
    gpioSetMode(RED_LED, PI_OUTPUT);
    gpioTerminate();

    return 0;
}
