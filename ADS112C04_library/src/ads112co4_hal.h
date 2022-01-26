#ifndef ADS112CO4_HAL_
#define ADS112CO4_HAL_
#include <stdint.h>

void i2c_init (void);
void i2c_read (uint8_t address, uint8_t *rxBuffer, uint8_t size);
void i2c_write (uint8_t address, uint8_t *txBuffer, uint8_t size);

void delay_ms(uint32_t milliseconds);

#endif