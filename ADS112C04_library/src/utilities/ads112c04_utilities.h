#ifndef ADS112C04_UTILITIES_H
#define ADS112C04_UTILITIES_H
#include <stdint.h>

/**
 * @brief Usefull to convert the internal sensor temperature data to Celsius degrees
 * 
 * @param raw_temperature_data data read from ADC when temperature sensor is enabled.
 * @return float Celsius degrees
 */
float ads112c04_rawTemperatureToCelsius(int16_t raw_temperature_data);

#endif 