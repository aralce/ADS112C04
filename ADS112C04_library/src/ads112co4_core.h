#ifndef ADS112CO4_CORE_
#define ADS112CO4_CORE_

#include <stdint.h>

#define ADS112CO4_TX_BUFFER_BYTES 1

typedef struct{
    uint8_t address;
}ads112co4_handler_t;

void ads112co4_init(ads112co4_handler_t *sensor_handler);
void ads112co4_reset(ads112co4_handler_t *sensor_handler);

#endif