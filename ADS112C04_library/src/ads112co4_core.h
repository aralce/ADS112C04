#ifndef ADS112CO4_CORE_
#define ADS112CO4_CORE_

#include <stdint.h>

#define ADS112CO4_TX_BUFFER_BYTES 1

typedef struct{
    uint8_t address;
}ads112co4_handler_t;

/**
 * @brief Inits the sensor and the I2C communications.
 * 
 * @param sensor_handler struct ads112co4_handler_t for dependency injection.
 */
void ads112co4_init(ads112co4_handler_t *sensor_handler);

/**
 * @brief Reset the internal configuration of the sensor to default.
 * 
 * @param sensor_handler struct ads112co4_handler_t for dependency injection.
 */
void ads112co4_reset(ads112co4_handler_t *sensor_handler);

/**
 * @brief Read the measured data from the sensor.
 * 
 * @param sensor_handler struct ads112co4_handler_t for dependency injection.
 * @return uint16_t 
 */
uint16_t ads112co4_readData(ads112co4_handler_t *sensor_handler);
#endif