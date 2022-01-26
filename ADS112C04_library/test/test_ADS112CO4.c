#include "unity.h"
#include "mock_ads112co4_hal.h"
#include "ads112co4_core.h"
#include "ads112co4_core_defines.h"
#include "ads112co4_suport.h"

/**
 * ads112c04_api:
 * -User interface
 * 
 * ads112c04_core:
 * -Controls the ads112C04 main functionality.
 * 
 * 
 * - Sensor is init.
 * - Sensor is resetted.
 * - Sensor data is read.
 * - Conversion mode is selected. 
 * 
 * - Operation mode is selected.
 * - Power-down mode is selected.
 * 
 */

ads112co4_handler_t sensor_handler;

void setup (void){}

void teardown (void){}

///////////////
//Sensor init
void test_sensor_init (void)
{
    //expect
    i2c_init_Expect ();
    delay_ms_Expect (SENSOR_INIT_DELAY_MS);
    //given
    ads112co4_init (&sensor_handler);
}

//////////////////////
//Sensor is resetted
void test_sensor_resseted (void)
{  
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112co4_init(&sensor_handler);
    //expect
    uint8_t txBuffer[1] = {COMMAND_RESET};
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    //given
    ads112co4_reset (&sensor_handler);
}

//////////////////////
//Sensor data is read
void test_sensor_data_read (void)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112co4_init(&sensor_handler);
    //expect
    uint8_t txBuffer[1] = {COMMAND_READ_DATA};
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    uint8_t rxBuffer[2];
    i2c_read_ExpectWithArray(sensor_handler.address, rxBuffer, 2, 2);
    TEST_ASSERT_EQUAL_UINT16 (0x0405, ads112co4_readData(&sensor_handler));
}

// ///////////////////////////////
// //Conversion mode is selected
// void test_sensor_conversion_mode_selected (void)
// {
//     sensor_init(&sensor_handler);
//     //expect

// }
