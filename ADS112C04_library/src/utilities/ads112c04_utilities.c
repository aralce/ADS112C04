#include "ads112c04_utilities.h"
#include <stdbool.h>

float ads112c04_rawTemperatureToCelsius(int16_t raw_temperature_data)
{
    bool is_negative = false;
    if (raw_temperature_data < 0){
        is_negative = true;
        raw_temperature_data *= -1; 
    }
    float degrees = (raw_temperature_data >> 2);
    return is_negative ? -degrees/32 : degrees/32;
}