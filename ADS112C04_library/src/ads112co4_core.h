#ifndef ADS112CO4_CORE_
#define ADS112CO4_CORE_

#include <stdint.h>
#include "ads112co4_api_config.h"

#define ADS112CO4_TX_BUFFER_BYTES 1

typedef struct{
    uint8_t address;
    uint8_t config0;
    uint8_t config1;
    uint8_t config2;
    uint8_t config3;
}ads112co4_handler_t;

typedef enum{
    CONTINUOS_CONVERSION = 1 /*!< 1 : Continuous conversion mode */
}ads112co4_conversion_mode_t;

typedef enum{
    NORMAL_MODE  /*!< Normal mode (256-kHz modulator clock, default) */
}ads112co4_operation_mode_t;

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

/**
 * @brief Change the sensor convertion mode
 * 
 * @param sensor_handler struct ads112co4_handler_t for dependency injection.
 * @param mode Normal mode(256-kHz modulator clock, default) - Turbo mode(512-kHz modulator clock) 
 */
void ads112co4_convertionMode(ads112co4_handler_t *sensor_handler, ads112co4_conversion_mode_t mode);

/**
 * @brief Change the senso operation mode
 * 
 * @param sensor_handler struct ads112co4_handler_t for dependency injection. 
 * @param mode Single-shot conversion mode(default) - Continuous convesion mode
 */
void ads112co4_operationMode(ads112co4_handler_t *sensor_handler, ads112co4_operation_mode_t mode);

void ads112co4_powerDown(ads112co4_handler_t *sensor_handler);

void ads112co4_startConversion(ads112co4_handler_t *sensor_handler);

void ads112co4_setAddress(ads112co4_handler_t *sensor_handler, uint8_t sensor_address);

uint8_t ads112co4_getAddress(ads112co4_handler_t *sensor_handler);
#endif