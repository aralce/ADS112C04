#ifndef ADS112C04_API_
#define ADS112C04_API_

#include <stdbool.h>
#include "ads112c04_api_config.h"
#include "ads112c04_api_defines.h"

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
 * @return false The conversion mode is not changed because the mode is invalid or the data was not written to sensor.
 */
bool ads112c04_conversionMode(ads112c04_handler_t *sensor_handler, ads112c04_conversion_mode_t mode);

/**
 * @brief Change the sensor operation mode
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection. 
 * @param mode Single-shot conversion mode(default) - Continuous conversion mode
 * @return true The function succeed.
 * @return false The operation mode is not changed because the mode is invalid or the data was not written to sensor.
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
 * @return false The reference voltage is not changed beacause ref is invalid or the data was not written to sensor.
 */
bool ads112c04_selectRefVoltage(ads112c04_handler_t *sensor_handler, ads112c04_ref_voltage_t ref);

/**
 * @brief Change the ADC data rate conversion in samples per second.
 *   If Turbo mode is active the selected data rate is x2.
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.
 * @param rate Samples per second desired. More samples per second more comsuption. 
 * @return true The data rate is changed
 * @return false The data rate is not changed because data rate is invalid or the data was not written to sensor.
 */
bool ads112c04_selectDataRate(ads112c04_handler_t *sensor_handler, ads112c04_data_rate_t rate);

/**
 * @brief Select the positive and negative input for conversion.
 *   The AVSS voltage can be selected as the negative input to do single-ended conversions.
 * @param sensor_handler struct ads112c04_handler_t for dependency injection. 
 * @param positive Input to connect to the positive side on ADC
 * @param negative Input to connect to the negative side on ADC
 * @return true  
 * @return false 
 */
bool ads112c04_selectInputs(ads112c04_handler_t *sensor_handler, ads112c04_input_t positive, ads112c04_input_t negative);

bool ads112c04_enterMonitorMode(ads112c04_handler_t *sensor_handler, ads112c04_monitor_mode_t mode);

bool ads112c04_setCalibrationMode(ads112c04_handler_t *sensor_handler);

bool ads112c04_selectGain(ads112c04_handler_t *sensor_handler, ads112c04_sensor_gain_t gain);

bool ads112c04_setPGA(ads112c04_handler_t *sensor_handler, ads112c04_PGA_status_t status);
#endif