#ifndef ADS112C04_CORE_
#define ADS112C04_CORE_

#include <stdint.h>
#include <stdbool.h>
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
    SINGLE_CONVERSION,      /*!< 0 : Single-shot conversion mode(default) */ 
    CONTINUOS_CONVERSION,   /*!< 1 : Continuous conversion mode */
    TOTAL_CONVERSION_MODES
}ads112c04_conversion_mode_t;

typedef enum{
    NORMAL_MODE,            /*!< 0 : Normal mode (256-kHz modulator clock, default) */
    TURBO_MODE,             /*!< 1 : Turbo mode (512-kHz modulator clock) */
    TOTAL_OPERATION_MODES
}ads112c04_operation_mode_t;

typedef enum{
    INTERNAL_VOLTAGE,         /*!< 0 : Internal 2.048-V reference selected (default) */
    REFP_REFN,                /*!< 1 : External reference selected using the REFP and REFN inputs */
    AVDD_AVSS,                /*!< 2 : Analog supply (AVDD – AVSS) used as reference */
    TOTAL_REFERENCE_VOLTAGES
}ads112c04_ref_voltage_t;

typedef enum{
    SPS_20,
    SPS_45,
    SPS_90,
    SPS_175,
    SPS_330,
    SPS_600,
    SPS_1000,
    TOTAL_DATA_RATES
}ads112c04_data_rate_t;

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
 * @return true The function succeed.
 * @return false The function fail in change the value. The read of the register is different from expected
 */
bool ads112c04_convertionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode);

/**
 * @brief Change the sensor operation mode
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection. 
 * @param mode Single-shot conversion mode(default) - Continuous conversion mode
 * @return true The function succeed.
 * @return false The function fail in change the value. The read of the register is different from expected
 */
bool ads112c04_operationMode(ads112c04_handler_t *sensor_handler, ads112c04_operation_mode_t mode);

/**
 * @brief Set the sensor in low power mode.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 */
void ads112c04_powerDown(ads112c04_handler_t *sensor_handler);

/**
 * @brief Start or Restart ADC conversion.
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

/**
 * @brief Change the reference voltage for ADC conversion.
 *   The selected voltage can be internal(default) or external.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @param ref The reference value to use in ADC conversion.
 * @return true The function succeed.
 * @return false The function fail in change the value. The read of the register is different from expected
 */
bool ads112c04_selectRefVoltage(ads112c04_handler_t *sensor_handler, ads112c04_ref_voltage_t ref);

bool ads112c04_selectDataRate(ads112c04_handler_t *sensor_handler, ads112c04_data_rate_t rate);
#endif