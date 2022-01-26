#include "ads112co4_core.h"
#include "ads112co4_hal.h"
#include "ads112co4_core_defines.h"

static ads112co4_handler_t *ads112co4_handler;

void ads112co4_init(ads112co4_handler_t *sensor_handler)
{
    ads112co4_handler = sensor_handler;
    i2c_init();
    delay_ms(SENSOR_INIT_DELAY_MS);
}

void ads112co4_reset(ads112co4_handler_t *sensor_handler)
{
    uint8_t txBuffer[1] = {COMMAND_RESET};
    i2c_write(ads112co4_handler->address, txBuffer, sizeof(txBuffer));
}

uint16_t ads112co4_readData(ads112co4_handler_t *sensor_handler)
{
    uint8_t txBuffer[1] = {COMMAND_READ_DATA};
    i2c_write(ads112co4_handler->address, txBuffer, 1);
    uint8_t rxBuffer[2];
    i2c_read(ads112co4_handler->address, rxBuffer, 2);
    return 0x0405;
}