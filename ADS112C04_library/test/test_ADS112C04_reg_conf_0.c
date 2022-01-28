#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "test_suport.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"


void setup (void){
    sensor_init(&sensor_handler);
}

void teardown (void)
{
    txBuffer[0] = 0;
    txBuffer[1] = 0;
}

