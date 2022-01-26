#ifndef ADS112CO4_SUPORT_
#define ADS112CO4_SUPORT_
#include "ads112co4_core.h"
#include "mock_ads112co4_hal.h"


inline void sensor_init(ads112co4_handler_t *sensor_handler)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112co4_init(sensor_handler);
}

#endif