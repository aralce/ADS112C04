#ifndef TEST_SUPORT_H_
#define TEST_SUPORT_H_
#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "ads112c04_api.h"
#include "ads112c04_core_defines.h"

#define NO_DATA    0

extern ads112c04_handler_t sensor_handler;
extern uint8_t txBuffer[2];

void sensor_init(ads112c04_handler_t *sensor_handler);
void i2c_sendCommand(uint8_t command, uint8_t data_add_with_command);
uint8_t sensor_checkRegister(uint8_t config_register_x_cm, uint8_t expected_return_value);

#endif