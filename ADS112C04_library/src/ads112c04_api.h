#ifndef ADS112C04_CORE_
#define ADS112C04_CORE_

#include <stdint.h>
#include "ads112c04_api_config.h"

#define ADS112C04_TX_BUFFER_BYTES 1

typedef struct{
    uint8_t address;
    uint8_t config0;
    uint8_t config1;
    uint8_t config2;
    uint8_t config3;
}ads112c04_handler_t;

typedef enum{
    CONTINUOS_CONVERSION = 1 /*!< 1 : Continuous conversion mode */
}ads112c04_conversion_mode_t;

typedef enum{
    NORMAL_MODE  /*!< Normal mode (256-kHz modulator clock, default) */
}ads112c04_operation_mode_t;

/**
 * @brief Inits the sensor and the I2C communications.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 */
void ads112c04_init(ads112c04_handler_t *sensor_handler);

/**
 * @brief Reset the internal configuration of the sensor to default.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 */
void ads112c04_reset(ads112c04_handler_t *sensor_handler);

/**
 * @brief Read the measured data from the sensor.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @return uint16_t 
 */
uint16_t ads112c04_readData(ads112c04_handler_t *sensor_handler);

/**
 * @brief Change the sensor convertion mode
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @param mode Normal mode(256-kHz modulator clock, default) - Turbo mode(512-kHz modulator clock) 
 */
void ads112c04_convertionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode);

/**
 * @brief Change the sensor operation mode
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection. 
 * @param mode Single-shot conversion mode(default) - Continuous conversion mode
 */
void ads112c04_operationMode(ads112c04_handler_t *sensor_handler, ads112c04_operation_mode_t mode);

/**
 * @brief Set the sensor in low power mode.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 */
void ads112c04_powerDown(ads112c04_handler_t *sensor_handler);

/**
 * @brief Start ADC conversion.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 */
void ads112c04_startConversion(ads112c04_handler_t *sensor_handler);

/**
 * @brief Set the sensor I2C address.
 *   The sensor has A0 and A1 pins to change the I2C directions.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @param sensor_address I2C device address. Be sure the addres math with your address device.
 *   The default address is: DEFAULT_SENSOR_ADDRESS. 
 */
void ads112c04_setAddress(ads112c04_handler_t *sensor_handler, uint8_t sensor_address);

/**
 * @brief Get the current I2C address configured in the library.
 *   Use this function to check the configured address match with the device address.
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @return uint8_t 
 */
uint8_t ads112c04_getAddress(ads112c04_handler_t *sensor_handler);

#endif