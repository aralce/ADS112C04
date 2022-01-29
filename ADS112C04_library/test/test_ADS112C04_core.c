#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"

#define NO_DATA    0

ads112c04_handler_t sensor_handler;
uint8_t txBuffer[2];

void sensor_init(ads112c04_handler_t *sensor_handler)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112c04_init(sensor_handler);
}

void setup (void){}

void teardown (void)
{
    txBuffer[0] = 0;
    txBuffer[1] = 0;
}

//Sensor init
void test_sensor_init (void)
{
    //expect
    i2c_init_Expect ();
    delay_ms_Expect (SENSOR_INIT_DELAY_MS);
    //given
    ads112c04_init (&sensor_handler);
}

//Sensor inits with default values
void test_sensor_init_with_default_values()
{
    //given
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL_HEX8 (DEFAULT_SENSOR_ADDRESS, sensor_handler.address);
    TEST_ASSERT_EQUAL_HEX8 (0, sensor_handler.config0);
    TEST_ASSERT_EQUAL_HEX8(0, sensor_handler.config1);
    TEST_ASSERT_EQUAL_HEX8(0, sensor_handler.config2);
    TEST_ASSERT_EQUAL_HEX8(0, sensor_handler.config3);
}

//Sensor is reset
void test_sensor_reset (void)
{  
    sensor_init(&sensor_handler);
    //expect
    txBuffer[0] = COMMAND_RESET;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    //given
    ads112c04_reset (&sensor_handler);
}

//Sensor data is read
void test_sensor_data_read (void)
{
    sensor_init(&sensor_handler);
    //expect
    txBuffer[0] = COMMAND_READ_DATA;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    i2c_read_ExpectAnyArgs();
    uint8_t rxBuffer[2] = {0x05, 0x04};
    i2c_read_ReturnArrayThruPtr_rxBuffer(rxBuffer, 2);
    //given
    TEST_ASSERT_EQUAL_HEX16(rxBuffer[0] + (rxBuffer[1] << 8), ads112c04_readData(&sensor_handler));
}

//Power-down mode is selected.
void test_sensor_power_down_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    txBuffer[0] = COMMAND_POWER_DOWN;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    //given
    ads112c04_powerDown(&sensor_handler);
}

//Sensor start or restart conversion
void test_sensor_start_or_restart_conversion (void)
{
    sensor_init(&sensor_handler);
    //expect
    txBuffer[0] = COMMAND_START_OR_SYNC;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    //given
    ads112c04_startConversion(&sensor_handler);
}

//The sensor address is set
void test_sensor_address_set (void)
{
    sensor_init(&sensor_handler);
    //given
    ads112c04_setAddress(&sensor_handler, 0X4F);
    //expect
    TEST_ASSERT_EQUAL_HEX8(0X4F, sensor_handler.address);
}

//The sensor address is gotten
void test_sensor_address_get (void)
{
    sensor_init(&sensor_handler);
    ads112c04_setAddress(&sensor_handler, 0X4F);
    //given
    uint8_t address = ads112c04_getAddress(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL_HEX8(0X4F, address);
}