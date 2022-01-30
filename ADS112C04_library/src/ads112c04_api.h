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
 *   A power reset on the sensor has the same effect.
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
 *   In low power mode the sensor is not taking measures.
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
 * @param positive Input to connect to the positive side on ADC. (default) AIN0
 * @param negative Input to connect to the negative side on ADC (default) AIN1
 * @return true The inputs changed.
 * @return false The inputs didn't changed because inputs are invalid or the data was not written to sensor.
 */
/*Table of valid combinations: 
 *|         |    nAIN0     |     nAIN1     |     nAIN2    |     nAIN3    |     nAVSS    | 
 *|=========|==============|===============|==============|==============|==============|
 *|  pAIN0  |INVALID_INPUT |     VALID     |     VALID    |     VALID    |     VALID    | 
 *|  pAIN1  |    VALID     | INVALID_INPUT |     VALID    |     VALID    |     VALID    |
 *|  pAIN2  |INVALID_INPUT | INVALID_INPUT |INVALID_INPUT |     VALID    |     VALID    | 
 *|  pAIN3  |INVALID_INPUT | INVALID_INPUT |     VALID    |INVALID_INPUT |     VALID    |
 *|  pAVSS  |INVALID_INPUT | INVALID_INPUT |INVALID_INPUT |INVALID_INPUT |INVALID_INPUT |
 *
 * + Prefix p for positive input.
 * + Prefix n for negative input.
 * + The default input is pAIN0/nAIN1 
 */
bool ads112c04_selectInputs(ads112c04_handler_t *sensor_handler, ads112c04_input_t positive, ads112c04_input_t negative);

/**
 * @brief The sensor changes its inputs to measure one of the external reference voltages.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection. 
 * @param mode Select beetwen (REFp-REFn)/4 and (AVDD-AVSS)/4 voltage.
 * @return true The inputs changed.
 * @return false The inputs didn't change because the mode is invalid or the data was not written to sensor. 
 */
bool ads112c04_enterMonitorMode(ads112c04_handler_t *sensor_handler, ads112c04_monitor_mode_t mode);

/**
 * @brief The sensor changes its inputs to measure the value (AVDD + AVSS) / 2  
 *  The internal multiplexer offers the option to short both PGA inputs to mid-supply (AVDD+AVSS)/2.
 *  This option can be used to measure and calibrate the device offset voltage by storing the result of the
 *  shorted input voltage reading and consequently subtracting the result from each following reading. 
 *  Take multiple readings with the inputs shorted and average the result to reduce the effect of noise.
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.  
 * @return true The inputs were changed to (AVDD+AVSS)/2
 * @return false The inputs didn't change because the data was not written to sensor.
 */
bool ads112c04_setCalibrationMode(ads112c04_handler_t *sensor_handler);

/**
 * @brief Select the ADC gain
 *  The PGA is always enabled for gain settings 8 to 128, regardless of the PGA_BYPASS set by setPGA function.
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.  
 * @param gain Gain from 1 to 128.  
 * @return true The ADC gain changed successfully.
 * @return false The ADC gain didn't changed because gain is invalid or the data was not written to sensor.
 */
bool ads112c04_selectGain(ads112c04_handler_t *sensor_handler, ads112c04_sensor_gain_t gain);

/**
 * @brief Enable or disable the Programmable Gain Amplifier.
 *   If the PGA is active the sensor comsumption is greater.
 *   Selecting a gain from 8 to 128 automatically enables the PGA.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.   
 * @param status PGA_ENABLED(default) of PGA_DISABLED
 * @return true The PGA status changed successfully. 
 * @return false The PGA status didn't change because the status is invalid or the data was not written to sensor.
 */
bool ads112c04_setPGA(ads112c04_handler_t *sensor_handler, ads112c04_PGA_status_t status);

/**
 * @brief Ask to the sensor if the ADC measure is ready.
 *   
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.    
 * @return true Data is ready
 * @return false Data is not ready
 */
bool ads112c04_checkDataReady(ads112c04_handler_t *sensor_handler);

/**
 * @brief The the current of both current sources.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.     
 * @param value current desired.
 * @return true the sensor changed the current.
 * @return false the sensor didn't change the current because value is not valid or the data was not written to sensor.
 */
bool ads112c04_setCurrent(ads112c04_handler_t *sensor_handler, ads112c04_current_t value);


/**
 * @brief Set the current output to one of the entries, REFp or REFn
 *   Both current sources can be output to the same place.
 * 
 * @param sensor_handler struct ads112c04_handler_t for dependency injection.     
 * @param source current source
 * @param output pin to output the current source
 * @return true the selected current source changed its output.
 * @return false the selected current source didn't change its output because the parameters are invalid or the data was not written to sensor.
 */
bool ads112c04_setCurrentOutput(ads112c04_handler_t *sensor_handler, ads112c04_current_source_t source, ads112c04_current_output_t output);

#endif