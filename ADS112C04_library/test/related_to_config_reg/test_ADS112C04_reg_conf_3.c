#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"

static ads112c04_handler_t sensor_handler;
static uint8_t txBuffer[2];

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

//The sensor change current source output IDAC1 and succeed
void test_sensor_current_source_output_IDAC1_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x02 << IDAC_1_OUTPUT_SHIFT);
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setCurrentOutput(&sensor_handler, IDAC1, 0x02));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config3);    
}

//The sensor change current source output IDAC1 and fail
void test_sensor_current_source_output_IDAC1_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x01 << IDAC_1_OUTPUT_SHIFT);
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_setCurrentOutput(&sensor_handler, IDAC1, 0x02));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(rx, sensor_handler.config3);    
}

//The sensor change current source output IDAC2 and succeed
void test_sensor_current_source_output_IDAC2_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x02 << IDAC_2_OUTPUT_SHIFT);
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setCurrentOutput(&sensor_handler, IDAC2, 0x02));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config3);    
}

//The sensor change current source output IDAC1 and fail
void test_sensor_current_source_output_IDAC2_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x01 << IDAC_2_OUTPUT_SHIFT);
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_setCurrentOutput(&sensor_handler, IDAC2, 0x02));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(rx, sensor_handler.config3);    
}

//Current source output: IDAC1 disabled
void test_sensor_current_source_output_IDAC1_disabled (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_DISABLED);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC1 connected to AIN0
void test_sensor_current_source_output_IDAC1_to_AIN0 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_TO_AIN0);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC1 connected to AIN1
void test_sensor_current_source_output_IDAC1_to_AIN1 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x02 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_TO_AIN1);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC1 connected to AIN2
void test_sensor_current_source_output_IDAC1_to_AIN2 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x03 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_TO_AIN2);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC1 connected to AIN3
void test_sensor_current_source_output_IDAC1_to_AIN3 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x04 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_TO_AIN3);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC1 connected to REFP
void test_sensor_current_source_output_IDAC1_to_REFP (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x05 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_TO_REFP);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC1 connected to REFN
void test_sensor_current_source_output_IDAC1_to_REFN (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x06 << IDAC_1_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC1, CURRENT_TO_REFN);
    //expect
    TEST_ASSERT_BITS(IDAC_1_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 disabled
void test_sensor_current_source_output_IDAC2_disabled (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_DISABLED);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 connected to AIN0
void test_sensor_current_source_output_IDAC2_to_AIN0 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_TO_AIN0);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 connected to AIN1
void test_sensor_current_source_output_IDAC2_to_AIN1 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x02 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_TO_AIN1);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 connected to AIN2
void test_sensor_current_source_output_IDAC2_to_AIN2 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x03 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_TO_AIN2);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 connected to AIN3
void test_sensor_current_source_output_IDAC2_to_AIN3 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x04 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_TO_AIN3);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 connected to REFP
void test_sensor_current_source_output_IDAC2_to_REFP (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x05 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_TO_REFP);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: IDAC2 connected to REFN
void test_sensor_current_source_output_IDAC2_to_REFN (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x06 << IDAC_2_OUTPUT_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_3_CM, data_mask);
    sensor_checkRegister(CONFIG_REGISTER_3_CM, data_mask);
    //given
    ads112c04_setCurrentOutput(&sensor_handler, IDAC2, CURRENT_TO_REFN);
    //expect
    TEST_ASSERT_BITS(IDAC_2_OUTPUT_MASK, data_mask, sensor_handler.config3);  
}

//Current source output: Output invalid
void test_sensor_current_source_output_to_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_setCurrentOutput(&sensor_handler, IDAC1, 0x07));
}

//Current source output: current source invalid
void test_sensor_current_source_output_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_setCurrentOutput(&sensor_handler, 0xFF, CURRENT_TO_AIN0));
}