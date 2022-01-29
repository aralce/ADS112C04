# ADS112C04  

## Description 
The ADS122C04 is a precision, 16-bit, analog-to-digital converter (ADC) that offers many integrated features to reduce system cost and component count in applications measuring small sensor signals. The device features two differential or four single-ended inputs through a flexible input multiplexer (MUX), a low-noise, programmable gain amplifier (PGA), two programmable excitation current sources, a voltage reference, an oscillator, and a precision temperature sensor.  

The device can perform conversions at data rates up to 2000 samples-per-second (SPS) with single-cycle settling. At 20 SPS, the digital filter offers simultaneous 50-Hz and 60-Hz rejection for noisy industrial applications. The internal PGA offers gains up to 128. This PGA makes the ADS112C04 ideally suited for applications measuring small sensor signals, such as resistance temperature detectors (RTDs), thermocouples, thermistors, and resistive bridge sensors. 

## How does it work?
![functional_diagram_block](https://user-images.githubusercontent.com/72839552/151069685-0ab7d038-0e03-4c45-b663-1d20fb2735c6.JPG)  
 - **MUX** *(input selection)*: The input can be selected from AIN0, AIN1, AIN2, AIN3, or AVSS. The MUX output is AINp(+) and AINn(-). The AVSS can only be selected for AINn(-) for single-ended measures.  Also, a temperature sensor can be used as an input: [temperature_sensor](https://user-images.githubusercontent.com/72839552/151074039-c37bb740-a61c-4d7f-b767-b82d83fcb35c.JPG)

 - **PGA**: The programmable gain amplifier can be configured to amplify the incoming signal. The system also uses a switched-capacitor to set the gain. [Gain description](https://user-images.githubusercontent.com/72839552/151073672-82be4148-f497-4c45-9524-05cfa7692905.JPG).  
 
 - **Programmable current source** (*IDAC*): There are 2 current sources that can set a current to the AINx pins and to REFP and REFN pins. They can be use to sense RTDs (resistance temperature detector).  

 - **Reference MUX**: The analog reference voltage can be selected from 3 different sources: build-in regulated source of 2.048v, REFP/REFN(analog external reference value), AVDD/AVSS(analog circuit power source).

 - **Two pins for configuring I2C address**: [address](https://user-images.githubusercontent.com/72839552/151076076-6c353bac-b2a4-471f-b377-95c38b8e0339.JPG)

 - **Pin for indicating is data ready** (~DRDY)
  
## What are the main libraries?  
The main ADS112C04 driver is provided by Texas Instruments as an example code:  [code.zip](https://github.com/aralce/ADS112C04/files/7937650/sbam381.zip)  
