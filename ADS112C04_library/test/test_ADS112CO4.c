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
 * - Operation mode is selected.
 * 
 * - Power-down mode is selected.
 * 
 */

ads112co4_handler_t sensor_handler;
uint8_t txBuffer[2];

void setup (void){}

void teardown (void)
{
    txBuffer[0] = 0;
    txBuffer[1] = 0;
}

void sensor_checkRegister(uint8_t config_register_x_cm)
{
    static uint8_t txBuffer[1] = {COMMAND_READ_REGISTER | CONFIG_REGISTER_1_CM};
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    i2c_read_ExpectAnyArgs();
    uint8_t rxBuffer[1] = {0x01};
    i2c_read_ReturnArrayThruPtr_rxBuffer(rxBuffer, 1);
}

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
void test_sensor_resset (void)
{  
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112co4_init(&sensor_handler);
    //expect
    txBuffer[0] = COMMAND_RESET;
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
    txBuffer[0] = COMMAND_READ_DATA;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    i2c_read_ExpectAnyArgs();
    uint8_t rxBuffer[2] = {0x05, 0x04};
    i2c_read_ReturnArrayThruPtr_rxBuffer(rxBuffer, 2);
    //given
    TEST_ASSERT_EQUAL_HEX16(rxBuffer[0] + (rxBuffer[1] << 8), ads112co4_readData(&sensor_handler));
}

///////////////////////////////
//Conversion mode is selected
void test_sensor_conversion_mode_selected (void)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112co4_init(&sensor_handler);
    //expect
    txBuffer[0] = COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM;
    txBuffer[1] = CONVERSION_MODE_MASK;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 2, 2);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112co4_convertionMode(&sensor_handler, CONTINUOS_CONVERSION);
    //expect
    TEST_ASSERT_BITS(CONVERSION_MODE_MASK, CONVERSION_MODE_MASK, sensor_handler.config1);
}

///////////////////////////
//Operation mode is selected
void test_sensor_operation_mode_selected (void)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112co4_init(&sensor_handler);
    //expect
    uint8_t txBuffer[2] = {COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, OPERATION_MODE_MASK};
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 2, 2);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112co4_operationMode(&sensor_handler, NORMAL_MODE);
    //expect
    TEST_ASSERT_BITS(OPERATION_MODE_MASK, NORMAL_MODE, sensor_handler.config1);
}