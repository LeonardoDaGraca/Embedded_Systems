#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define RED_LED 21

/*
 gcc -pthread -o mcp3008 mcp3008.c -lpigpio -lrt
 mcp3008 [num_of_loops] [speed_of_transmission]
 defaults to 1000000 in both cases
*/
int main(int argc, char *argv[]) {
    int i;
    int h;
    int v;
    int loops;
    int speed;
    double start, diff, sps;
    unsigned char buf[3];
    if (argc > 1) loops = atoi(argv[1]);
    else loops = 1000000;
    if (argc > 2) speed = atoi(argv[2]);
    else speed = 1000000;
    if (gpioInitialise() < 0) return 1;
    h = spiOpen(0, speed, 0);
    if (h < 0) return 2;
    start = time_time();

    gpioSetMode(RED_LED, PI_OUTPUT);

    for (i = 0; i < loops; i++) {
        buf[0] = 1;
        buf[1] = 128;
        buf[2] = 0;
        spiXfer(h, buf, buf, 3);
        v = ((buf[1] & 3) << 8) | buf[2];
        printf("%d\n", v);
        int pwm = (int) ((1023 - v) / 1024.0 * 256);
        gpioPWM(RED_LED, pwm);
    }
    diff = time_time() - start;
    fprintf(stderr, "sps=%.1f @ %d bps (%d/%.1f)\n",
            (double) loops / diff, speed, loops, diff);
    spiClose(h);
    gpioSetMode(RED_LED, PI_OUTPUT);
    gpioTerminate();
    return 0;
}