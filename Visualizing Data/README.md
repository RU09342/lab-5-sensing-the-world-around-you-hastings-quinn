# Visualizing Data: Philip Quinn and Sean Hastings

This set of C code is designed to allow for a sensor voltage to be read into one of the 12-bit ADCs of an MSP430FR6989 and displayed on its LCD. This implemetation utilizes A4 located at pin P8.7. To modify which ADC is used, modify lines 13, 14, and 24 to use whichever ADC desired. Refer to the microcontroller's datasheet provided by Texas Insrument's (TI) for more information on what ADCs are available. This code is most efficiently implemented in TI's IDE Code Composer Studio (CCS). Connect the output of your sensor into the P8.7 or whichever ADC header of your MSP, download each of the three files found inside of the LCD folder, and run the main.c file in CCS in order to have your sensor signal displayed. An example of this is shown below.

![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-hastings-quinn/blob/master/Visualizing%20Data/FullSizeR.jpg)

When reading in the temperature from the ADC, it is split into 3 4 bit chunks in order for the temperature to be properly read on the LCD screen. Within the code you will also find that the temperature readings can also be read over UART. 