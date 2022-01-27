#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"

#define NO_DATA    0

ads112c04_handler_t sensor_handler;
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

void sensor_init(ads112c04_handler_t *sensor_handler)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112c04_init(sensor_handler);
}

void i2c_sendCommand(uint8_t command, uint8_t data_add_with_command)
{
    static uint8_t txBuffer[2];
    txBuffer[0] = command;
    txBuffer[1] = data_add_with_command;
    uint8_t size = 1;
    if(COMMAND_WRITE_REGISTER == (command & 0xF0)) //Only the last 4 bits indicates the COMMAND_WRITE_REGISTER
        size = 2;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, size, size);
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

//Sensor change conversion mode and succeed
void test_sensor_conversion_mode_succeed (void)
{
    TEST_FAIL_MESSAGE("Next test to implement");
}

//Conversion mode: Continuos mode is selected.
void test_sensor_conversion_mode_continuos_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | 1*CONVERSION_MODE_MASK);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112c04_convertionMode(&sensor_handler, CONTINUOS_CONVERSION);
    //expect
    TEST_ASSERT_BITS(CONVERSION_MODE_MASK, CONVERSION_MODE_MASK, sensor_handler.config1);
}

//Conversion mode: Single-shot mode is selected
void test_sensor_conversion_mode_singleShot_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | 0*CONVERSION_MODE_MASK);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112c04_convertionMode(&sensor_handler, SINGLE_CONVERSION);
    //expect
    TEST_ASSERT_BITS(CONVERSION_MODE_MASK, 0, sensor_handler.config1);

}

//Operation mode: normal mode is selected.
void test_sensor_operation_mode_normal_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | 0*OPERATION_MODE_MASK);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112c04_operationMode(&sensor_handler, NORMAL_MODE);
    //expect
    TEST_ASSERT_BITS(OPERATION_MODE_MASK, NORMAL_MODE, sensor_handler.config1);
}

//Operation mode: turbo mode is selected.
void test_sensor_operation_mode_turbo_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | 1*OPERATION_MODE_MASK);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112c04_operationMode(&sensor_handler, TURBO_MODE);
    //expect
    TEST_ASSERT_BITS(OPERATION_MODE_MASK, TURBO_MODE, sensor_handler.config1);
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

//voltage reference: AVDD_AVSS is selected
void test_sensor_reference_voltage_AVDD_AVSS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (AVDD_AVSS << VOLTAGE_REFERENCE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM);
    //given
    ads112c04_selectRefVoltage(&sensor_handler, AVDD_AVSS);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config1);
}