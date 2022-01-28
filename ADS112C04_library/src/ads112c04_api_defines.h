#ifndef ADS112c04_API_DEFINES_
#define ADS112c04_API_DEFINES_
#include <stdint.h>

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

typedef enum{
    AIN0,
    AIN1,
    AIN2,
    AIN3,
    AVSS,
    TOTAL_INPUTS
}ads112c04_input_t;

typedef enum{
    MONITOR_VREFP_VREFN = 0x0C,
    MONITOR_AVDD_AVSS,
}ads112c04_monitor_mode_t;

typedef enum{
    SENSOR_GAIN_1   =   1,
    SENSOR_GAIN_2   =   2,
    SENSOR_GAIN_4   =   4,
    SENSOR_GAIN_8   =   8,
    SENSOR_GAIN_16  =  16,
    SENSOR_GAIN_32  =  32,
    SENSOR_GAIN_64  =  64,
    SENSOR_GAIN_128 = 128
}ads112c04_sensor_gain_t;

typedef enum{
    PGA_ENABLED,
    PGA_DISABLED
}ads112c04_PGA_status_t;

#endif