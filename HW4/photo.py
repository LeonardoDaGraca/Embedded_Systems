'''
Leonardo DaGraca
HW4: External Signaling via Photocell

Original python code obtained from professor's
HW assignment. Tweaked pins to match my board 
configuration.
'''

import RPi.GPIO as GPIO, time, os, sys     

#Settings
IN_RC= 17       #Input pin
OUT_LOW = 16    #Low-light output
OUT_HIGH = 20   #High-light output
OUT_STATE = 21  #Program state output

  

DEBUG = 1
GPIO.setmode(GPIO.BCM)
GPIO.setup(27, GPIO.OUT)  #Setting photocell resistor to as an output
GPIO.output(27, GPIO.HIGH) #Turning pin voltage to high
GPIO.setup(OUT_LOW, GPIO.OUT)
GPIO.setup(OUT_HIGH, GPIO.OUT)
GPIO.setup(OUT_STATE, GPIO.OUT)


def RCtime (RCpin):
        reading = 0
        GPIO.setup(RCpin, GPIO.OUT)
        GPIO.output(RCpin, GPIO.LOW)
        time.sleep(0.1)

        GPIO.setup(RCpin, GPIO.IN)
        # This takes about 1 millisecond per loop cycle
        while (GPIO.input(RCpin) == GPIO.LOW):
                reading += 1
        return reading

def ledOut(state):
    if state == 0:
        GPIO.output(OUT_LOW, True)
        GPIO.output(OUT_HIGH, True)
        return 0
    if state == 1:
        GPIO.output(OUT_LOW, False)
        GPIO.output(OUT_HIGH, True)
        return 0
    if state == 2:
        GPIO.output(OUT_LOW, True)
        GPIO.output(OUT_HIGH, False)
        return 0
    if state == 3:
        GPIO.output(OUT_LOW, False)
        GPIO.output(OUT_HIGH, False)
        return 0
    return 1

def photocellParse(reading):
    out = ""
    if reading <= 15000:
        out = "0"
        ledOut(0)
        return out
    if reading <= 20000:
        out = "o"
        ledOut(1)
        return out
    if reading <= 75000:
        out = "."
        ledOut(2)
        return out
    out = " "
    ledOut(3)
    return out


try:
	while True:
	        GPIO.output(OUT_STATE, True)
	        sys.stdout.write(photocellParse(RCtime(IN_RC)))    # Read RC timing using RC_IN pin, parse it, and spit it into stdout
	        sys.stdout.flush()
	        GPIO.output(OUT_STATE, False)
finally:
	GPIO.cleanup()
