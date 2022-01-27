#ifndef ADS112C04_HAL_
#define ADS112C04_HAL_
#include <stdint.h>

/* --Insert your user includes here-- */

/* --Insert your user includes here-- */


/**
 * @brief HAL function to init I2C
 *  Please, replace the inside content with the required code to init your
 *  I2C peripheral in your hardware.
 * 
 *  What is it expected?
 *  -The I2C can read data and write data with underneath functions.
 * 
 */
void i2c_init (void);

/**
 * @brief HAL function to read data with I2C
 *  Please, replace the inside content with the required code to read data
 *  with your I2C peripheral in your hardware.
 * 
 *  What is it expected?
 *  -The I2C read data from the address and request as many bytes as size parameter.
 *  -The *rxBuffer is a pointer to the first element of an array provided by user.   
 * 
 * @param address 
 * @param rxBuffer 
 * @param size 
 */
void i2c_read (uint8_t address, uint8_t *rxBuffer, uint8_t size);

/**
 * @brief HAL function to write data with I2C
 *  Please, replace the inside content with the required code to write data
 *  with your I2C peripheral in your hardware.
 * 
 *  What is it expected?
 *  -The I2C write data to the address and write as many bytes as size parameter.
 *  -The *txBuffer is a pointer to the first element of an array provided by user.
 * @param address 
 * @param txBuffer 
 * @param size 
 */
void i2c_write (uint8_t address, uint8_t *txBuffer, uint8_t size);

/**
 * @brief HAL function for blocking delay
 *   Please, replace the inside content with the required code to write data
 *   with your I2C peripheral in your hardware.
 * 
 *   What is it expected?
 *   -The delay_ms would block the current task for milliseconds.
 *   -For example the function can be filled with vTaskDelay function 
 *   of the freeRTOS or delay function from Arduino.
 * @param milliseconds 
 */
void delay_ms(uint32_t milliseconds);

#endif