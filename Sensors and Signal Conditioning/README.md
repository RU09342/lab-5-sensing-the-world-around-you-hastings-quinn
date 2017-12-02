# Sensors and Signal Conditioning

In this lab sensors are used to record environmental measurements to be read by the MSP430G2553's Analog-to-Digital converter. The sensors used were a photoresistor, photodiode, and phototransistor. The code for the MSP430 simply reads from the ADC port once a second and transmits over UART.

## Sensors

Three different types of sensor outputs are used in this lab: voltage, current and resistance. Interestingly enough, all of the sensors used were light based, as the prefix "photo" suggests. The photo resistor converts the amount of light it sees into a resistance whereas the photodiode outputs a voltage and the phototransistor outputs a current. The photoresistor was used in a voltage divider circuit with another resistor. As the light increased, the photoresistor's resistance decreased, and when the light decreased, the resistance increased. For the photodiode, it was simply modeled as a voltage source. Thus, as light exposure increased, so did output voltage. The phototransistor was slightly more complicated. This device required a transimpedance amplifier in order to convert current output to a voltage. 

## ADC

The MSP430G2553 has a 10 bit analog-to-digital convertor that can be configured using associated pins as defined in the family datasheet. In this lab, pin 1.1 was used to take in an analog voltage reading from the output of a sensor circuit. The ADC is sampled once a second to update voltage measurements, which are then transmitted over a UART serial connection with a 9600 Baud Rate. Since the ADC is 10 bits and UART can only transmit a single byte at a time, the ADC memory register needs to be bit shifted and transmitted two times to fully send the buffer.

## Testing

To fully test the functionality of the hardware and software implemented in this lab, each sensor circuit was isolated and then integrated with the MSP430G2553. A DMM was used first to measure the voltage output of each circuit while varying the light intensity using a phone's flashlight at different distances and angles. All of the circuits behaved as predicted so they were then each tested using the ADC of the MSP. By reading the ADC values over UART and comparing with values shown on the DMM, it was confirmed that the ADC code was working correctly.

## Schematics

Phototransistors emmit a current out of their collector pin based on the intensity of the light taken in. Below is the schematic of the circit used to convert current into a voltage to be read by the ADC of the MSP430G2553. It forces a positive voltage at the output of an Op-Amp that is valued as the current output of the phototransistor multiplied by the value of the feedback resistor. In the schematic shown below, the phototransistor is modeled as a constant current source, but in practice the collector of the phototransistor would be connected to ground, while the emitter is tied to inverting terminal of the Op-Amp.

### Transimpedance amplifier modeled with current 10uA and feedback resistance of 1M Ohms.
![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-hastings-quinn/blob/master/Sensors%20and%20Signal%20Conditioning/Current2VPNG.PNG)

Photoresistors change their resistances based on the intensity of the light taken in. Below is a schematic showing its placement into a voltage divider for the purpose of scaling the output voltage as a function of the resistance of the photoresistor: Vo = Vin(R2/R2+R1); where R2 is the resistance of the photoresistor. The schematics below show examples of the photoresistor affecting at large and small values directly affecting the voltage output.

### Photoresistor set to 1M Ohms to show low light intensity's effect on output voltage.
![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-hastings-quinn/blob/master/Sensors%20and%20Signal%20Conditioning/R2VVHigh.PNG)

### Photoresistor set to 1m Ohms to show high light intensity's effect on output voltage.
![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-hastings-quinn/blob/master/Sensors%20and%20Signal%20Conditioning/R2VVLowPNG.PNG)

As mentioned above, the photodiode can be modeled as a voltage source depending on the intensity of light it is exposed to. Within the schematic shown below, the voltage source at the output of the voltage divider represents the photodiode. It is fed a voltage divider of the 3.3v VCC to ensure that the output voltage of the photodiode is between 1.65 and 3.3, which is the input limit of the G2553. 

### Photodiode modeled as a voltage source.

![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-hastings-quinn/blob/master/Sensors%20and%20Signal%20Conditioning/V2VSchematic.PNG)
