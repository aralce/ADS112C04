/**
 * Thermocuple exmample base on ADS112C04 datasheet.
 */
#include "ads112c04_api.h"
#include "ads112c04_hal.h" //to use delay_ms

void main(void)
{
    ads112c04_handler_t sensor_handler; //creates the sensor struct
    ads112c04_init(&sensor_handler); //inits sensor

    ads112c04_setGain(&sensor_handler, SENSOR_GAIN_8); //set the PGA gain to 8

    ads112c04_setRefVoltage(&sensor_handler, REFP_REFN); //set the voltage reference as external.

    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, AIN2); //set IDAC1 for output on AIN2
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, AIN3); //set IDAC2 for output on AIN3

    //At this point we can change conversion mode to continuos conversion. For the porpose of the example will change the mode.
    ads112c04_setConversionMode(&sensor_handler, CONTINUOS_CONVERSION);

    ads112c04_startConversion(&sensor_handler);   
    while(1){
        //To know when the data is ready we can set an interrupt or read on pin ~(DRDY) of the chip or just query it to the sensor. 
        if ( ads112c04_checkDataReady(&sensor_handler) ){
            uint16_t data = ads112c04_readData(&sensor_handler);
            printf("Data: %d\r\n", data); //The data value is referenced to the reference voltage. Take care with the Full Scale Range.
        }else
            delay_ms(20);
    }
}