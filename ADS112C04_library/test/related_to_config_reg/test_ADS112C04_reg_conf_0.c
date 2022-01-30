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

//Sensor input change and keeps other configurations on config register 0
void test_sensor_input_keep_other_configs (void)
{
    sensor_init(&sensor_handler);
    sensor_handler.config0 = 0xFF;
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = sensor_handler.config0 & (~INPUT_SELECTION_MASK); // AINp = AIN0 and ANn = AIN
    sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_selectInputs(&sensor_handler, AIN0, AIN1));
    //expect
    TEST_ASSERT_EQUAL_HEX8( data_mask, sensor_handler.config0);     
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
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
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Input: invalid input combination si selected, then function fails
void test_sensor_input_invalid_combination_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_selectInputs(&sensor_handler, AIN2, AIN1));
}

//Input: IN+ is not a valid input.
void test_sensor_input_INp_is_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_selectInputs(&sensor_handler, TOTAL_INPUTS, AVSS));
}

//Input: IN- is not a valid input.
void test_sensor_input_INn_is_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_selectInputs(&sensor_handler, AIN0, TOTAL_INPUTS));
}

//Sensor enters in monitor mode change and succeed
void test_sensor_enter_monitor_mode_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x0C << INPUT_SELECTION_SHIFT) & INPUT_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_enterMonitorMode(&sensor_handler, MONITOR_VREFP_VREFN));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config0);     
}

//Sensor enters in monitor mode change and keeps other configurations on config register 0
void test_sensor_enter_monitor_mode_and_keep_other_configs (void)
{
    sensor_init(&sensor_handler);
    sensor_handler.config0 = 0xFF;
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = sensor_handler.config0 & (~INPUT_SELECTION_MASK);
    data_mask |= MONITOR_VREFP_VREFN << INPUT_SELECTION_SHIFT;
    sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_enterMonitorMode(&sensor_handler, MONITOR_VREFP_VREFN));
    //expect
    TEST_ASSERT_EQUAL_HEX8( data_mask, sensor_handler.config0);      
}

//Sensor enters in monitor mode change and fails
void test_sensor_enter_monitor_mode_fails (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x0D << INPUT_SELECTION_SHIFT) & INPUT_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_enterMonitorMode(&sensor_handler, MONITOR_VREFP_VREFN));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(rx, sensor_handler.config0);     
}

//Monitor mode: monitor VREFP_VREFN
void test_sensor_enter_monitor_mode_monitor_VREFP_VREFN (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x0C << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_enterMonitorMode(&sensor_handler, MONITOR_VREFP_VREFN);
    //expect
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Monitor mode: monitor AVDD_AVSS
void test_sensor_enter_monitor_mode_monitor_AVDD_AVSS (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x0D << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_enterMonitorMode(&sensor_handler, MONITOR_AVDD_AVSS);
    //expect
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Monitor mode: monitor value is invalid
void test_sensor_enter_monitor_mode_monitor_value_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_enterMonitorMode(&sensor_handler, 0));
}

//Sensor sets calibration mode and succeed
void test_sensor_set_calibration_mode_succeed()
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x0E << INPUT_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setCalibrationMode(&sensor_handler));
    //expect
    TEST_ASSERT_BITS(INPUT_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor sets calibration mode and keeps other configurations on config register 0
void test_sensor_set_calibration_mode_and_keep_other_configs (void)
{
    sensor_init(&sensor_handler);
    sensor_handler.config0 = 0xFF;
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = sensor_handler.config0 & (~INPUT_SELECTION_MASK);
    data_mask |= 0x0E << INPUT_SELECTION_SHIFT; //introduce the value for calibration mode.
    sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setCalibrationMode(&sensor_handler));
    //expect
    TEST_ASSERT_EQUAL_HEX8( data_mask, sensor_handler.config0);     
}

//Sensor sets calibration mode and fails
void test_sensor_set_calibration_mode_fail()
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x0F << INPUT_SELECTION_SHIFT);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_setCalibrationMode(&sensor_handler));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(0x0E, sensor_handler.config0);
}

//Sensor change gain and succeed
void test_sensor_gain_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_1));
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain and keeps other configurations on config register 0
void test_sensor_gain_change_and_keep_other_configs (void)
{
    sensor_init(&sensor_handler);
    sensor_handler.config0 = 0xFF;
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = sensor_handler.config0 & (~GAIN_SELECTION_MASK); //set gain selection to reg value 0x00 -> gain 1
    sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_1));
    //expect
    TEST_ASSERT_EQUAL_HEX8( data_mask, sensor_handler.config0);     
}

//Sensor change gain and fails.
void test_sensor_gain_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (SENSOR_GAIN_2 << GAIN_SELECTION_SHIFT);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_1));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(data_mask, sensor_handler.config0);
}

//Sensor change gain to 1
void test_sensor_gain_select_gain_1 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_1);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 2
void test_sensor_gain_select_gain_2 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_2);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 4
void test_sensor_gain_select_gain_4 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x02 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_4);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 8
void test_sensor_gain_select_gain_8 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x03 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_8);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 16
void test_sensor_gain_select_gain_16 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x04 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_16);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 32
void test_sensor_gain_select_gain_32 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x05 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_32);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 64
void test_sensor_gain_select_gain_64 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x06 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_64);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to 128
void test_sensor_gain_select_gain_128 (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x07 << GAIN_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    ads112c04_selectGain(&sensor_handler, SENSOR_GAIN_128);
    //expect
    TEST_ASSERT_BITS(GAIN_SELECTION_MASK, data_mask, sensor_handler.config0);
}

//Sensor change gain to invalid value
void test_sensor_gain_select_invalid_value (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_selectGain(&sensor_handler, 38));
}

//Sensor set PGA and succeed
void test_sensor_set_PGA_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = PGA_BYPASS_MASK;
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setPGA(&sensor_handler, 1));
    //expect
    TEST_ASSERT_BITS( PGA_BYPASS_MASK, PGA_BYPASS_MASK, sensor_handler.config0);
}

//Sensor set PGA and keeps other configurations on config register 0
void test_sensor_set_PGA_and_keep_other_configs (void)
{
    sensor_init(&sensor_handler);
    sensor_handler.config0 = 0xFF;
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = sensor_handler.config0 & (~PGA_BYPASS_MASK);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setPGA(&sensor_handler, 0));
    //expect
    TEST_ASSERT_EQUAL_HEX8( 0xFF & (~PGA_BYPASS_MASK), sensor_handler.config0);    
}

//Sensor set PGA and fails
void test_sensor_set_PGA_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = PGA_DISABLED << PGA_BYPASS_SHIFT;
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_setPGA(&sensor_handler, PGA_ENABLED));
    //expect
    TEST_ASSERT_NOT_EQUAL_HEX8(data_mask, sensor_handler.config0);
}

//Sensor set PGA to PGA enabled
void test_sensor_set_PGA_enabled (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x00 << PGA_BYPASS_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setPGA(&sensor_handler, PGA_ENABLED));
    //expect
    TEST_ASSERT_BITS(PGA_BYPASS_MASK, data_mask, sensor_handler.config0);
}

//Sensor set PGA to PGA disabled
void test_sensor_set_PGA_disabled (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (0x01 << PGA_BYPASS_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_0_CM, sensor_handler.config0 | data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_setPGA(&sensor_handler, PGA_DISABLED));
    //expect
    TEST_ASSERT_BITS(PGA_BYPASS_MASK, data_mask, sensor_handler.config0);
}

//Sensor set PGA to invalid value
void test_sensor_set_PGA_invalid (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_setPGA(&sensor_handler, 0x05));
}