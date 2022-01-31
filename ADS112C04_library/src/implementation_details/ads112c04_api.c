#include "../ads112c04_api.h"
#include "../ads112c04_hal.h"
#include "ads112c04_core_defines.h"

#define countof(x) sizeof(x)/sizeof(x[0])

#define NO_MASK             0
#define NO_REGISTER_VALUE   0
#define INVALID_INPUT       0xFF
#define CM_TO_NUM           2

/* ==== [Private function declaration] ====================================================== */
static void send_command(ads112c04_handler_t *sensor_handler, uint8_t command, uint8_t config_register_x_cm, uint8_t register_value);
static bool change_config_reg_and_check(ads112c04_handler_t *sensor_handler, uint8_t config_register_x_cm, uint8_t register_value);
static void reset_config_registers(ads112c04_handler_t *sensor_handler);

/* ==== [Public function definition] ======================================================== */
void ads112c04_init(ads112c04_handler_t *sensor_handler) 
{
    sensor_handler->address = DEFAULT_SENSOR_ADDRESS;
    i2c_init();
    delay_ms(SENSOR_INIT_DELAY_MS);
    ads112c04_reset(sensor_handler);
}

void ads112c04_reset(ads112c04_handler_t *sensor_handler)
{
    send_command(sensor_handler, COMMAND_RESET, NO_MASK, NO_REGISTER_VALUE);
    sensor_handler->config0 = 0;
    sensor_handler->config1 = 0;
    sensor_handler->config2 = 0;
    sensor_handler->config3 = 0;
}

uint16_t ads112c04_readData(ads112c04_handler_t *sensor_handler)
{
    send_command(sensor_handler, COMMAND_READ_DATA, NO_MASK, NO_REGISTER_VALUE);
    uint8_t rxBuffer[2];
    i2c_read(sensor_handler->address, rxBuffer, 2);    
    return ((rxBuffer[1] << 8) + rxBuffer[0]);
}

bool ads112c04_setConversionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode)
{
    if(mode >= TOTAL_CONVERSION_MODES)
        return false;
    uint8_t data_mask = (mode << CONVERSION_MODE_SHIFT) | (sensor_handler->config1 &(~CONVERSION_MODE_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
}

bool ads112c04_setOperationMode(ads112c04_handler_t *sensor_handler, ads112c04_operation_mode_t mode)
{
    if(mode >= TOTAL_OPERATION_MODES)
        return false;
    uint8_t data_mask = (mode << OPERATION_MODE_SHIFT) | (sensor_handler->config1 &(~OPERATION_MODE_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
}

void ads112c04_powerDown(ads112c04_handler_t *sensor_handler)
{
    send_command(sensor_handler, COMMAND_POWER_DOWN, NO_MASK, NO_REGISTER_VALUE);
}

void ads112c04_startConversion(ads112c04_handler_t *sensor_handler)
{
    send_command(sensor_handler, COMMAND_START_OR_SYNC, NO_MASK, NO_REGISTER_VALUE);  
}

void ads112c04_setAddress(ads112c04_handler_t *sensor_handler, uint8_t sensor_address)
{
    sensor_handler->address = sensor_address;
}

uint8_t ads112c04_getAddress(ads112c04_handler_t *sensor_handler)
{
    return sensor_handler->address;
}

bool ads112c04_setRefVoltage(ads112c04_handler_t *sensor_handler, ads112c04_ref_voltage_t ref)
{
    if(ref >= TOTAL_REFERENCE_VOLTAGES)
        return false;
    uint8_t data_mask = (ref << VOLTAGE_REFERENCE_SELECTION_SHIFT) | (sensor_handler->config1 &(~VOLTAGE_REFERENCE_SELECTION_MASK)); 
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
}

bool ads112c04_setDataRate(ads112c04_handler_t *sensor_handler, ads112c04_data_rate_t rate)
{
    if(rate >= TOTAL_DATA_RATES)
        return false;
    uint8_t data_mask = (rate << DATA_RATE_SELECTION_SHIFT) | (sensor_handler->config1 &(~DATA_RATE_SELECTION_MASK)); 
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
}

static const uint8_t input_matrix_AINp_AINn_to_value[5][5] =  //ROW is POSITIVE , COLUMN is NEGATIVE// 
//        |    nAIN0     |     nAIN1     |     nAIN2    |     nAIN3    |     nAVSS    | 
/*pAIN0*/{{ INVALID_INPUT,      0x00     ,     0x01     ,     0x02     ,     0x08     }, /*pAIN0 */  
/*pAIN1 */{     0x03     , INVALID_INPUT ,     0x04     ,     0x05     ,     0x09     }, /*pAIN1 */
/*pAIN2 */{INVALID_INPUT , INVALID_INPUT ,INVALID_INPUT ,     0x06     ,     0x0A     }, /*pAIN2 */
/*pAIN3 */{INVALID_INPUT , INVALID_INPUT ,     0x07     ,INVALID_INPUT ,     0x0B     }, /*pAIN3 */
/*pAVSS */{INVALID_INPUT , INVALID_INPUT ,INVALID_INPUT ,INVALID_INPUT ,INVALID_INPUT }};/*pAVSS */
bool ads112c04_setInputs(ads112c04_handler_t *sensor_handler, ads112c04_input_t positive, ads112c04_input_t negative)
{
    if(positive >= TOTAL_INPUTS || negative >= TOTAL_INPUTS)
        return false;
    if(input_matrix_AINp_AINn_to_value[positive][negative] == INVALID_INPUT)
        return false;
    uint8_t data_mask = input_matrix_AINp_AINn_to_value[positive][negative] << INPUT_SELECTION_SHIFT;
    data_mask |= sensor_handler->config0 &(~INPUT_SELECTION_MASK);
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
}

bool ads112c04_setMonitorMode(ads112c04_handler_t *sensor_handler, ads112c04_monitor_mode_t mode)
{
    if( mode != MONITOR_VREFP_VREFN && mode != MONITOR_AVDD_AVSS)
        return false;
    uint8_t data_mask = (mode << INPUT_SELECTION_SHIFT) | (sensor_handler->config0 &(~INPUT_SELECTION_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
}

bool ads112c04_setCalibrationMode(ads112c04_handler_t *sensor_handler)
{
    uint8_t data_mask = (0x0E << INPUT_SELECTION_SHIFT) | (sensor_handler->config0 &(~INPUT_SELECTION_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
}

bool ads112c04_setGain(ads112c04_handler_t *sensor_handler, ads112c04_sensor_gain_t gain)
{
    uint8_t gain_index = 0;
    static const uint8_t array_of_gains[] = { SENSOR_GAIN_1 , SENSOR_GAIN_2 , SENSOR_GAIN_4 , SENSOR_GAIN_8,
                                              SENSOR_GAIN_16, SENSOR_GAIN_32, SENSOR_GAIN_64, SENSOR_GAIN_128};
    for( ; gain_index<=countof(array_of_gains); ++gain_index){ //extract gain_index from gain
        if(gain_index == countof(array_of_gains) || gain == array_of_gains[gain_index]) //if first condition is true second is not evaluated
            break;
    }
    if(gain_index == countof(array_of_gains)) //invalid value
        return false;
    uint8_t data_mask = (gain_index << GAIN_SELECTION_SHIFT) | (sensor_handler->config0 &(~GAIN_SELECTION_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
}

bool ads112c04_setPGA(ads112c04_handler_t *sensor_handler, ads112c04_PGA_status_t status)
{
    if(status >= ALL_PGA_STATUS)
        return false;
    uint8_t data_mask = (status << PGA_BYPASS_SHIFT) | (sensor_handler->config0 &(~PGA_BYPASS_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
}

bool ads112c04_checkDataReady(ads112c04_handler_t *sensor_handler)
{
   uint8_t txBuffer[1] = {COMMAND_READ_REGISTER | CONFIG_REGISTER_2_CM};
   i2c_write(sensor_handler->address, txBuffer, 1);
   uint8_t rxBuffer[1];
   i2c_read(sensor_handler->address, rxBuffer, 1);
   if(rxBuffer[0] == 0)
    return false;
   return true; 
}

bool ads112c04_setCurrent(ads112c04_handler_t *sensor_handler, ads112c04_current_t value)
{
    uint8_t current_index = 0;
    static const uint16_t array_of_currents[] = { CURRENT_0_uA  , CURRENT_10_uA , CURRENT_50_uA  , CURRENT_100_uA,
                                                  CURRENT_250_uA, CURRENT_500_uA, CURRENT_1000_uA, CURRENT_1500_uA};
    for( ; current_index<=countof(array_of_currents); ++current_index){ //extract index
        if(current_index == countof(array_of_currents) || value == array_of_currents[current_index]) //if first condition is true second is not evaluated
            break;
    }
    if(current_index == countof(array_of_currents)) //invalid value
        return false;
    uint8_t data_mask = (current_index << CURRENT_VALUE_SHIFT) | (sensor_handler->config2 &(~CURRENT_VALUE_MASK));
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_2_CM, data_mask);
}

bool ads112c04_setCurrentOutput(ads112c04_handler_t *sensor_handler, ads112c04_current_source_t source, ads112c04_current_output_t output)
{
    if(source >= TOTAL_CURRENT_SOURCES || output >= TOTAL_CURRENT_OUTPUTS)
        return false;
    uint8_t data_mask;
    if(source == IDAC1)
        data_mask = (output << IDAC_1_OUTPUT_SHIFT) | (sensor_handler->config3 &(~IDAC_1_OUTPUT_MASK)); 
    else
        data_mask = (output << IDAC_2_OUTPUT_SHIFT) | (sensor_handler->config3 &(~IDAC_2_OUTPUT_MASK));
    
    return change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_3_CM, data_mask);
}

/* ==== [Private function definition] ===================================================== */
/**
 * @brief Send the selected command by I2C
 * 
 * @param command defined command
 * @param command_mask_for_reg Is the mask applied to the command. The commands to write and read registers need specify the register. 
 * @param register_value The value to write to the config value. If the command don't need this its value doesn't matter.
 */
static void send_command(ads112c04_handler_t *sensor_handler, uint8_t command, uint8_t config_register_x_cm, uint8_t register_value)
{
    uint8_t private_command = command;
    uint8_t size = 1; //by default sent only the command
    uint8_t txBuffer[2];
    if(command == COMMAND_READ_REGISTER || command == COMMAND_WRITE_REGISTER)
        private_command |= config_register_x_cm; //config register to merge with command
    if(command == COMMAND_WRITE_REGISTER){
        size = 2; //needs to send data for register.
        txBuffer[1] = register_value;
    }
    txBuffer[0] = private_command;
    i2c_write(sensor_handler->address, txBuffer, size);
}

/**
 * @brief Change the value of the config register and check the value changed.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @param config_register_x_cm  Is the mask applied to the command. The commands to write and read registers need specify the register.
 * @param register_value The value to write to the config register.
 * @return true The register changed
 * @return false The register didn't change because the data was not written to the sensor.
 */
static bool change_config_reg_and_check(ads112c04_handler_t *sensor_handler, uint8_t config_register_x_cm, uint8_t register_value)
{
    uint8_t *config_array[] = { &(sensor_handler->config0), &(sensor_handler->config1), &(sensor_handler->config2),
                                &(sensor_handler->config3)};
    send_command(sensor_handler, COMMAND_WRITE_REGISTER, config_register_x_cm, register_value);
    send_command(sensor_handler, COMMAND_READ_REGISTER, config_register_x_cm, NO_REGISTER_VALUE);
    uint8_t rxBuffer[1];
    i2c_read(sensor_handler->address, rxBuffer, 1);
    if(rxBuffer[0] != register_value)
        return false;
    *config_array[config_register_x_cm >> CM_TO_NUM] = rxBuffer[0]; //use config_register_x_cm to know to what config to write.
    return true;    
}