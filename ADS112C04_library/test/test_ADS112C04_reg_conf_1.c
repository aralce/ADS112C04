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

//Sensor change conversion mode and succeed
void test_sensor_conversion_mode_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, CONVERSION_MODE_MASK);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_convertionMode(&sensor_handler, 1));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config1);
}

//Sensor change conversion mode and fail
void test_sensor_conversion_mode_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, CONVERSION_MODE_MASK);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_convertionMode(&sensor_handler, 0));
    //expect
    TEST_ASSERT_NOT_EQUAL_UINT8(rx, sensor_handler.config1);
}

//Conversion mode: invalid value is selected
void test_sensor_conversion_mode_invalid_value_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_convertionMode(&sensor_handler, TOTAL_CONVERSION_MODES));
}

//Conversion mode: Continuos mode is selected.
void test_sensor_conversion_mode_continuos_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | 1*CONVERSION_MODE_MASK);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | 1*CONVERSION_MODE_MASK);
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
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | 0*CONVERSION_MODE_MASK);
    //given
    ads112c04_convertionMode(&sensor_handler, SINGLE_CONVERSION);
    //expect
    TEST_ASSERT_BITS(CONVERSION_MODE_MASK, 0, sensor_handler.config1);

}

//Sensor change operation mode and succeed
void test_sensor_operation_mode_succeed (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, OPERATION_MODE_MASK);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_operationMode(&sensor_handler, 1));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config1);    
}

//Sensor change operation mode and fail
void test_sensor_operation_mode_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, OPERATION_MODE_MASK);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_operationMode(&sensor_handler, 0));
    //expect
    TEST_ASSERT_NOT_EQUAL_UINT8(rx, sensor_handler.config1);    
}

//Operation mode: invalid value is selected.
void test_sensor_operation_mode_invalid_value_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_operationMode(&sensor_handler, TOTAL_OPERATION_MODES));
}

//Operation mode: normal mode is selected.
void test_sensor_operation_mode_normal_mode_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | 0*OPERATION_MODE_MASK);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | 0*OPERATION_MODE_MASK);
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
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | 0*OPERATION_MODE_MASK);
    //given
    ads112c04_operationMode(&sensor_handler, TURBO_MODE);
    //expect
    TEST_ASSERT_BITS(OPERATION_MODE_MASK, TURBO_MODE, sensor_handler.config1);
}

//Sensor Voltage reference change and succeed
void test_sensor_reference_voltage_succeed (void)
{  
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x02 << VOLTAGE_REFERENCE_SELECTION_SHIFT) & VOLTAGE_REFERENCE_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_selectRefVoltage(&sensor_handler, 0x02));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config1); 
}

//Sensor Voltage reference change and fail
void test_sensor_reference_voltage_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x03 << VOLTAGE_REFERENCE_SELECTION_SHIFT) & VOLTAGE_REFERENCE_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_selectRefVoltage(&sensor_handler, 0x02));
    //expect
    TEST_ASSERT_NOT_EQUAL_UINT8(rx, sensor_handler.config1); 
}

//Voltage reference: invalid value is selected.
void test_sensor_reference_voltage_invalid_value_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_selectRefVoltage(&sensor_handler, TOTAL_REFERENCE_VOLTAGES));    
}

//Voltage reference: internal voltage is selected.
void test_sensor_reference_voltage_internal_reference_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (INTERNAL_VOLTAGE << VOLTAGE_REFERENCE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectRefVoltage(&sensor_handler, INTERNAL_VOLTAGE);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config1);      
}

//Voltage reference: REFP_REFN is selected.
void test_sensor_reference_voltage_REFP_REFN_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (REFP_REFN << VOLTAGE_REFERENCE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectRefVoltage(&sensor_handler, REFP_REFN);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config1);    
}

//voltage reference: AVDD_AVSS is selected
void test_sensor_reference_voltage_AVDD_AVSS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (AVDD_AVSS << VOLTAGE_REFERENCE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectRefVoltage(&sensor_handler, AVDD_AVSS);
    //expect
    TEST_ASSERT_BITS(VOLTAGE_REFERENCE_SELECTION_MASK, data_mask, sensor_handler.config1);
}

//The sensor change the data rate and succeed.
void test_sensor_data_rate_succeed (void)
{  
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x03 << DATA_RATE_SELECTION_SHIFT) & DATA_RATE_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(true, ads112c04_selectDataRate(&sensor_handler, 0x03));
    //expect
    TEST_ASSERT_EQUAL_HEX8(rx, sensor_handler.config1); 
}

//The sensor  change the data rate and fail
void test_sensor_data_rate_fail (void)
{
    sensor_init(&sensor_handler);
    //expect
    i2c_write_ExpectAnyArgs();
    uint8_t data_mask = (0x03 << DATA_RATE_SELECTION_SHIFT) & DATA_RATE_SELECTION_MASK;
    uint8_t rx = sensor_checkRegister(CONFIG_REGISTER_1_CM, data_mask);
    //given
    TEST_ASSERT_EQUAL(false, ads112c04_selectDataRate(&sensor_handler, 0x02));
    //expect
    TEST_ASSERT_NOT_EQUAL_UINT8(rx, sensor_handler.config1);   
}

//Data rate: invalid value is selected
void test_sensor_data_rate_invalid_value_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    TEST_ASSERT_EQUAL(false, ads112c04_selectDataRate(&sensor_handler, TOTAL_DATA_RATES));     
}

//Data rate:  20SPS is selected
void test_sensor_data_rate_20_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_20 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectRefVoltage(&sensor_handler, SPS_20);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);      
}

//Data rate:  45SPS is selected
void test_sensor_data_rate_45_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_45 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectDataRate(&sensor_handler, SPS_45);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);  
}

//Data rate:  90SPS is selected
void test_sensor_data_rate_90_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_90 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectDataRate(&sensor_handler, SPS_90);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);  
}

//Data rate: 175SPS is selected
void test_sensor_data_rate_175_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_175 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectDataRate(&sensor_handler, SPS_175);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);      
}

//Data rate: 330SPS is selected
void test_sensor_data_rate_330_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_330 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectDataRate(&sensor_handler, SPS_330);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);      
}

//Data rate: 600SPS is selected
void test_sensor_data_rate_600_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_600 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectDataRate(&sensor_handler, SPS_600);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);      
}

//Data rate: 1000SPS is selected
void test_sensor_data_rate_1000_SPS_selected (void)
{
    sensor_init(&sensor_handler);
    //expect
    uint8_t data_mask = (SPS_1000 << DATA_RATE_SELECTION_SHIFT);
    i2c_sendCommand(COMMAND_WRITE_REGISTER | CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    sensor_checkRegister(CONFIG_REGISTER_1_CM, sensor_handler.config1 | data_mask);
    //given
    ads112c04_selectDataRate(&sensor_handler, SPS_1000);
    //expect
    TEST_ASSERT_BITS(DATA_RATE_SELECTION_MASK, data_mask, sensor_handler.config1);      
}