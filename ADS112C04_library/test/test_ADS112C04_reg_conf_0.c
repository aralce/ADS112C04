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

//Sensor input change and succeed
void test_sensor_input_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x01 << INPUT_SELECTION_SHIFT) & INPUT_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_selectInputs(&sensor_handler, AIN0, AIN2));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config0); 
}

//Sensor input change and fail in change register
void test_sensor_input_fail_change_register (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x02 << INPUT_SELECTION_SHIFT) & INPUT_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_selectInputs(&sensor_handler, AIN0, AIN2));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(rx, sensor_handler.config0); 
}

//Input: IN+ is AIN0 and IN- is AIN1 is selected
void test_sensor_input_INpAIN0_INnAIN1 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN0, AIN1);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN0 and IN- is AIN2 is selected
void test_sensor_input_INpAIN0_INnAIN2 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN0, AIN2);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN0 and IN- is AIN3 is selected
void test_sensor_input_INpAIN0_INnAIN3 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x02 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN0, AIN3);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN1 and IN- is AIN0 is selected
void test_sensor_input_INpAIN1_INnAIN0 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x03 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN1, AIN0);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN1 and IN- is AIN2 is selected
void test_sensor_input_INpAIN1_INnAIN2 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x04 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN1, AIN2);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN1 and IN- is AIN3 is selected
void test_sensor_input_INpAIN1_INnAIN3 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x05 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN1, AIN3);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN2 and IN- is AIN3 is selected
void test_sensor_input_INpAIN2_INnAIN3 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x06 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN2, AIN3);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN3 and IN- is AIN2 is selected
void test_sensor_input_INpAIN3_INnAIN2 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x07 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN3, AIN2);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN0 and IN- is AVSS is selected
void test_sensor_input_INpAIN0_INnAVSS (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x08 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN0, AVSS);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN1 and IN- is AVSS is selected
void test_sensor_input_INpAIN1_INnAVSS (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x09 << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN1, AVSS);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN2 and IN- is AVSS is selected
void test_sensor_input_INpAIN2_INnAVSS (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x0A << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN2, AVSS);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: IN+ is AIN3 and IN- is AVSS is selected
void test_sensor_input_INpAIN3_INnAVSS (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x0B << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectInputs(&sensor_handler, AIN3, AVSS);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config0);
}