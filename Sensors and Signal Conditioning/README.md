# Sensors and Signal Conditioning

In this lab sensors are used to record environmental measurements to be read by the MSP430G2553's Analog-to-Digital converter. The sensors used were a photoresistor, photodiode, and phototransistor. The code for the MSP430 simply reads from the ADC port once a second and transmits over UART.

## Sensors

Three different types of sensor outputs are used in this lab: voltage, current and resistance. Interestingly enough, all of the sensors used were light based, as the prefix "photo" suggests. The photo resistor converts the amount of light it sees into a resistance whereas the photodiode outputs a voltage and the phototransistor outputs a current. The photoresistor was used in a voltage divider circuit with another resistor. As the light increased, the photoresistor's resistance decreased, and when the light decreased, the resistance increased. For the photodiode, it was simply modeled as a voltage source. Thus, as light exposure increased, so did output voltage. The phototransistor was slightly more complicated. This device required a transimpedance amplifier in order to convert current output to a voltage. A full schematic of the sensor circuits can be seen in the picture below.

## ADC

The MSP430G2553 has a 10 bit analog-to-digital convertor that can be configured using associated pins as defined in the family datasheet. In this lab, pin 1.1 was used to take in an analog voltage reading from the output of a sensor circuit. The ADC is sampled once a second to update voltage measurements, which are then transmitted over a UART serial connection with a 9600 Baud Rate. Since the ADC is 10 bits and UART can only transmit a single byte at a time, the ADC memory register needs to be bit shifted and transmitted two times to fully send the buffer.

## Testing

To fully test the functionality of the hardware and software implemented in this lab, each sensor circuit was isolated and then integrated with the MSP430G2553. A DMM was used first to measure the voltage output of each circuit while varying the light intensity using a phone's flashlight at different distances and angles. All of the circuits behaved as predicted so they were then each tested using the ADC of the MSP. By reading the ADC values over UART and comparing with values shown on the DMM, it was confirmed that the ADC code was working correctly.


### README
The README for this part of the lab should talk briefly about how the ADC code works, but focus way more on the hardware aspect. You need to include schematics of your circuits, and well as supporting simulation/calculations which show that your circuits should work. You should talk about what types of circuits you are using and how they actually work.