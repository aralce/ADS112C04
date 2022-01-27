#include "ads112c04_api.h"
#include "ads112c04_hal.h"
#include "ads112c04_core_defines.h"

#define NO_MASK_TO_APPLY    0
#define NO_REGISTER_VALUE   0

static ads112c04_handler_t *ads112c04_handler;

/* ==== [Private function declaration] ====================================================== */
static void send_command(uint8_t command, uint8_t command_mask_for_reg, uint8_t register_value);


/* ==== [Public function definition] ======================================================== */
void ads112c04_init(ads112c04_handler_t *sensor_handler)
{
    ads112c04_handler = sensor_handler;
    ads112c04_handler->address = DEFAULT_SENSOR_ADDRESS;
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

void ads112c04_convertionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode)
{
    send_command(COMMAND_WRITE_REGISTER, CONFIG_REGISTER_1_CM, sensor_handler->config1 | CONVERSION_MODE_MASK);
    send_command(COMMAND_READ_REGISTER, CONFIG_REGISTER_1_CM, NO_REGISTER_VALUE);
    uint8_t rxBuffer[1];
    i2c_read(sensor_handler->address, rxBuffer, 1);
    sensor_handler->config1 |= 0xff & CONVERSION_MODE_MASK; 
}

void ads112c04_operationMode(ads112c04_handler_t *sensor_handler, ads112c04_operation_mode_t mode)
{
    send_command(COMMAND_WRITE_REGISTER, OPERATION_MODE_MASK, sensor_handler->config1 | OPERATION_MODE_MASK);
    send_command(COMMAND_READ_REGISTER, CONFIG_REGISTER_1_CM, NO_REGISTER_VALUE);
    uint8_t rxBuffer[1];
    i2c_read(sensor_handler->address, rxBuffer, 1);
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