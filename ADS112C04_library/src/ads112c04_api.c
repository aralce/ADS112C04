#include "ads112c04_api.h"
#include "ads112c04_hal.h"
#include "ads112c04_core_defines.h"

#define countof(x) sizeof(x)/sizeof(x[0])

#define NO_MASK             0
#define NO_REGISTER_VALUE   0
#define INVALID_INPUT       0xFF

/* ==== [Private global variables] =============================================================== */
static ads112c04_handler_t *ads112c04_handler;

/* ==== [Private function declaration] ====================================================== */
static void send_command(uint8_t command, uint8_t config_register_x_cm, uint8_t register_value);
static uint8_t change_config_reg_and_check(ads112c04_handler_t *sensor_handler, uint8_t config_register_x_cm, uint8_t register_value);

/* ==== [Public function definition] ======================================================== */
void ads112c04_init(ads112c04_handler_t *sensor_handler) 
{
    ads112c04_handler = sensor_handler;
    ads112c04_handler->address = DEFAULT_SENSOR_ADDRESS;
    ads112c04_handler->config0 = 0;
    ads112c04_handler->config1 = 0;
    i2c_init();
    delay_ms(SENSOR_INIT_DELAY_MS);
}

void ads112c04_reset(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_RESET, NO_MASK, NO_REGISTER_VALUE);
}

uint16_t ads112c04_readData(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_READ_DATA, NO_MASK, NO_REGISTER_VALUE);
    uint8_t rxBuffer[2];
    i2c_read(ads112c04_handler->address, rxBuffer, 2);    
    return ((rxBuffer[1] << 8) + rxBuffer[0]);
}

bool ads112c04_conversionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode)
{
    if(mode >= TOTAL_CONVERSION_MODES)
        return false;
    uint8_t data_mask = (mode << CONVERSION_MODE_SHIFT);
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx != data_mask)
        return false;
    sensor_handler->config1 = rx;
    return true;
}

bool ads112c04_operationMode(ads112c04_handler_t *sensor_handler, ads112c04_operation_mode_t mode)
{
    if(mode >= TOTAL_OPERATION_MODES)
        return false;
    uint8_t data_mask = mode > 0 ? OPERATION_MODE_MASK : 0;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx != data_mask)
        return false;
    sensor_handler->config1 = rx;
    return true;
}

void ads112c04_powerDown(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_POWER_DOWN, NO_MASK, NO_REGISTER_VALUE);
}

void ads112c04_startConversion(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_START_OR_SYNC, NO_MASK, NO_REGISTER_VALUE);  
}

void ads112c04_setAddress(ads112c04_handler_t *sensor_handler, uint8_t sensor_address)
{
    sensor_handler->address = sensor_address;
}

uint8_t ads112c04_getAddress(ads112c04_handler_t *sensor_handler)
{
    return sensor_handler->address;
}

bool ads112c04_selectRefVoltage(ads112c04_handler_t *sensor_handler, ads112c04_ref_voltage_t ref)
{
    if(ref >= TOTAL_REFERENCE_VOLTAGES)
        return false;
    uint8_t data_mask = (ref << VOLTAGE_REFERENCE_SELECTION_SHIFT); 
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx != data_mask)
        return false;
    sensor_handler->config1 |= data_mask;
    return true;
}

bool ads112c04_selectDataRate(ads112c04_handler_t *sensor_handler, ads112c04_data_rate_t rate)
{
    if(rate >= TOTAL_DATA_RATES)
        return false;
    uint8_t data_mask = (rate << DATA_RATE_SELECTION_SHIFT); 
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx != data_mask)
        return false;
    sensor_handler->config1 = rx;
    return true;
}

static const uint8_t input_matrix_AINp_AINn_to_value[5][5] =  //ROW is POSITIVE , COLUMN is NEGATIVE// 
//        |    nAIN0     |     nAIN1     |     nAIN2    |     nAIN3    |     nAVSS    | 
/*pAIN0*/{{ INVALID_INPUT,      0x00     ,     0x01     ,     0x02     ,     0x08     }, /*pAIN0 */  
/*pAIN1 */{     0x03     , INVALID_INPUT ,     0x04     ,     0x05     ,     0x09     }, /*pAIN1 */
/*pAIN2 */{INVALID_INPUT , INVALID_INPUT ,INVALID_INPUT ,     0x06     ,     0x0A     }, /*pAIN2 */
/*pAIN3 */{INVALID_INPUT , INVALID_INPUT ,     0x07     ,INVALID_INPUT ,     0x0B     }, /*pAIN3 */
/*pAVSS */{INVALID_INPUT , INVALID_INPUT ,INVALID_INPUT ,INVALID_INPUT ,INVALID_INPUT }};/*pAVSS */
bool ads112c04_selectInputs(ads112c04_handler_t *sensor_handler, ads112c04_input_t positive, ads112c04_input_t negative)
{
    if(positive >= TOTAL_INPUTS || negative >= TOTAL_INPUTS)
        return false;
    if(input_matrix_AINp_AINn_to_value[positive][negative] == INVALID_INPUT)
        return false;
    uint8_t data_mask = input_matrix_AINp_AINn_to_value[positive][negative] << INPUT_SELECTION_SHIFT;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
    if(rx != data_mask)
        return false;
    sensor_handler->config0 = rx;
    return true;
}

bool ads112c04_enterMonitorMode(ads112c04_handler_t *sensor_handler, ads112c04_monitor_mode_t mode)
{
    if( mode != MONITOR_VREFP_VREFN && mode != MONITOR_AVDD_AVSS)
        return false;
    uint8_t data_mask = mode << INPUT_SELECTION_SHIFT;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
    if( rx != data_mask)
        return false;
    sensor_handler->config0 = rx;
    return true;
}

bool ads112c04_setCalibrationMode(ads112c04_handler_t *sensor_handler)
{
    uint8_t data_mask = 0x0E << INPUT_SELECTION_SHIFT;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
    if( rx != data_mask)
        return false;
    sensor_handler->config0 = rx;
    return true;
}

bool ads112c04_selectGain(ads112c04_handler_t *sensor_handler, ads112c04_sensor_gain_t gain)
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
    uint8_t data_mask = gain_index << GAIN_SELECTION_SHIFT;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
    if( rx != data_mask)
        return false;
    sensor_handler->config0 = rx;
    return true;
}

bool ads112c04_setPGA(ads112c04_handler_t *sensor_handler, ads112c04_PGA_status_t status)
{
    uint8_t data_mask = status << PGA_BYPASS_SHIFT;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_0_CM, data_mask);
    if( rx != data_mask)
        return false;
    sensor_handler->config0 = rx;
    return true;
}

/* ==== [Private function definition] ===================================================== */
/**
 * @brief Send the selected command by I2C
 * 
 * @param command defined command
 * @param command_mask_for_reg Is the mask applied to the command. The commands to write and read registers need specify the register. 
 * @param register_value 
 */
static void send_command(uint8_t command, uint8_t config_register_x_cm, uint8_t register_value)
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
    i2c_write(ads112c04_handler->address, txBuffer, size);
}

/**
 * @brief Change the value of the config register and check the value changed.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @param config_register_x_cm  Is the mask applied to the command. The commands to write and read registers need specify the register.
 * @param register_value The value to write to the config register.
 * @return uint8_t 
 */
static uint8_t change_config_reg_and_check(ads112c04_handler_t *sensor_handler, uint8_t config_register_x_cm, uint8_t register_value)
{
    send_command(COMMAND_WRITE_REGISTER, config_register_x_cm, register_value);
    send_command(COMMAND_READ_REGISTER, config_register_x_cm, NO_REGISTER_VALUE);
    uint8_t rxBuffer[1];
    i2c_read(sensor_handler->address, rxBuffer, 1);
    return rxBuffer[0];
}
