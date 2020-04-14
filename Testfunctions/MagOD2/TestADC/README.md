# TestADC.ino
Leon Abelmann, April 2020

Program to test the ADS1115 converters on the MagOD2 board.
Will read the ADC values, send them to the serial port and display
them on the TFTM50 screen. Calculates the standard deviation of N
measurements, and max-min values (only serial output).

Requires MagOD_ADS115, which is a modification of the Adafruit ADS1015
library to incorporate interupt based readout. You can set the
samplingrate (sampleRate).

ADC0 is read using interupt on port 13 of ESP32 (connected to
ALERT/RDY pin of ADS1115)

ADC1 is read using a suitable delay.
