#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"

ads112c04_handler_t sensor_handler;

static void sensor_init(ads112c04_handler_t *sensor_handler)
{
    i2c_init_Ignore();
    delay_ms_Ignore();
    ads112c04_init(sensor_handler);
}

static void i2c_sendCommand(uint8_t command, uint8_t data_add_with_command)
{
    static uint8_t txBuffer[2];
    txBuffer[0] = command;
    txBuffer[1] = data_add_with_command;
    uint8_t size = 1;
    if(COMMAND_WRITE_REGISTER == (command & 0xF0)) //Only the last 4 bits indicates the COMMAND_WRITE_REGISTER
        size = 2;
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, size, size);
}

static uint8_t sensor_checkRegister(uint8_t config_register_x_cm, uint8_t expected_return_value)
{
    static uint8_t txBuffer[1]; 
    txBuffer[0] = COMMAND_READ_REGISTER | config_register_x_cm; //CONFIG REGISTERS TO MERGE WITH REGISTER COMMANDS
    i2c_write_ExpectWithArray (sensor_handler.address, txBuffer, 1, 1);
    i2c_read_ExpectAnyArgs();
    uint8_t rxBuffer[1];
    rxBuffer[0] = expected_return_value;
    i2c_read_ReturnArrayThruPtr_rxBuffer(rxBuffer, 1);
    return rxBuffer[0];
}

void setup (void){}

void teardown (void){}

//The sensor query if data is ready and data is ready
void test_sensor_query_if_data_ready_the_data_is_ready (void) 
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << DATA_READY_SHIFT);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, 0x01);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_checkDataReady(&sensor_handler));
}

//The sensor query if data is ready and data is not ready
void test_sensor_query_if_data_ready_the_data_is_not_ready (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << DATA_READY_SHIFT);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, 0x00);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_checkDataReady(&sensor_handler));    
}

//The sensor set the current for IDAC1_IDAC2 and succeed
void test_sensor_set_current_IDAC1_IDAC2_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x02 << CURRENT_VALUE_SHIFT);
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_2_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setCurrent(&sensor_handler, CURRENT_50_uA));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config2);
}

//The sensor set the current for IDAC1_IDAC2 and fails
void test_sensor_set_current_IDAC1_IDAC2_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x01 << CURRENT_VALUE_SHIFT);
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_2_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_setCurrent(&sensor_handler, CURRENT_50_uA));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(rx, sensor_handler.config2);
}

//Source current:    0uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_0uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_0_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current:   10uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_10uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_10_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current:   50uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_50uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x02 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_50_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current:  100uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_100uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x03 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_100_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current:  250uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_250uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x04 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_250_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

///Source current:  500uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_500uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x05 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_500_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current: 1000uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_1000uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x06 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_1000_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current: 1500uA is selected.
void test_sensor_set_current_IDAC1_IDAC2_to_1500uA (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x07 << CURRENT_VALUE_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_2_CM, sensor_handler.config2 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_2_CM, sensor_handler.config2| data_mask);
    //given
    ads112c04_setCurrent(&sensor_handler, CURRENT_1500_uA);
    //expect
    TEST_ASSERT_BITS(CURRENT_VALUE_MASK, data_mask, sensor_handler.config2);      
}

//Source current: invalid current is selected.
void test_sensor_set_current_IDAC1_IDAC2_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_setCurrent(&sensor_handler, 0xFF));
}