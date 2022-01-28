#include "ads112c04_api.h"
#include "ads112c04_hal.h"
#include "ads112c04_core_defines.h"

#define NO_MASK_TO_APPLY    0
#define NO_REGISTER_VALUE   0

static ads112c04_handler_t *ads112c04_handler;

/* ==== [Private function declaration] ====================================================== */
static void send_command(uint8_t command, uint8_t command_mask_for_reg, uint8_t register_value);
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
    send_command(COMMAND_RESET, NO_MASK_TO_APPLY, NO_REGISTER_VALUE);
}

uint16_t ads112c04_readData(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_READ_DATA, NO_MASK_TO_APPLY, NO_REGISTER_VALUE);
    uint8_t rxBuffer[2];
    i2c_read(ads112c04_handler->address, rxBuffer, 2);    
    return ((rxBuffer[1] << 8) + rxBuffer[0]);
}

bool ads112c04_convertionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode)
{
    if(mode >= TOTAL_CONVERSION_MODES)
        return false;
    uint8_t data_mask = (mode << CONVERSION_MODE_SHIFT);
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx == (sensor_handler->config1 | data_mask))
        sensor_handler->config1 = rx;
    else
        return false;
    return true;
}

bool ads112c04_operationMode(ads112c04_handler_t *sensor_handler, ads112c04_operation_mode_t mode)
{
    if(mode >= TOTAL_OPERATION_MODES)
        return false;
    uint8_t data_mask = mode > 0 ? OPERATION_MODE_MASK : 0;
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx == (sensor_handler->config1 | data_mask))
        sensor_handler->config1 = rx;
    else
        return false;
    return true;
}

void ads112c04_powerDown(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_POWER_DOWN, NO_MASK_TO_APPLY, NO_REGISTER_VALUE);
}

void ads112c04_startConversion(ads112c04_handler_t *sensor_handler)
{
    send_command(COMMAND_START_OR_SYNC, NO_MASK_TO_APPLY, NO_REGISTER_VALUE);  
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
    if(rx == (sensor_handler->config1 | data_mask))
        sensor_handler->config1 |= data_mask;
    else
        return false;
    return true;
}

bool ads112c04_selectDataRate(ads112c04_handler_t *sensor_handler, ads112c04_data_rate_t rate)
{
    if(rate >= TOTAL_DATA_RATES)
        return false;
    uint8_t data_mask = (rate << DATA_RATE_SELECTION_SHIFT); 
    uint8_t rx = change_config_reg_and_check(sensor_handler, CONFIG_REGISTER_1_CM, data_mask);
    if(rx == (sensor_handler->config1 | data_mask))
        sensor_handler->config1 = rx;
    else
        return false;
    return true;
}

/* ==== [Private function definition] ===================================================== */
static void send_command(uint8_t command, uint8_t command_mask_for_reg, uint8_t register_value)
{
    uint8_t private_command = command;
    uint8_t size = 1; //by default sent only the command
    uint8_t txBuffer[2];
    if(command == COMMAND_READ_REGISTER || command == COMMAND_WRITE_REGISTER)
        private_command |= command_mask_for_reg; //config register to merge with command
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
