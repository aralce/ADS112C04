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
    SINGLE_CONVERSION,      /*!< 0 :(default) Single-shot conversion mode(default) */ 
    CONTINUOS_CONVERSION,   /*!< 1 : Continuous conversion mode */
    TOTAL_CONVERSION_MODES
}ads112c04_conversion_mode_t;

typedef enum{
    NORMAL_MODE,            /*!< 0 :(default) Normal mode (256-kHz modulator clock, default) */
    TURBO_MODE,             /*!< 1 : Turbo mode (512-kHz modulator clock) */
    TOTAL_OPERATION_MODES
}ads112c04_operation_mode_t;

typedef enum{
    INTERNAL_VOLTAGE,       /*!< 0 :(default) Internal 2.048-V reference selected (default) */
    REFP_REFN,              /*!< 1 : External reference selected using the REFP and REFN inputs */
    AVDD_AVSS,              /*!< 2 : Analog supply (AVDD – AVSS) used as reference */
    TOTAL_REFERENCE_VOLTAGES
}ads112c04_ref_voltage_t;

typedef enum{
    SPS_20,                 /*!< 0 :(default) 20 samples per second when Normal mode is active. 40SPS with Turbo mode */         
    SPS_45,                 /*!< 1 : 45 samples per second when Normal mode is active. 90SPS with Turbo mode */         
    SPS_90,                 /*!< 2 : 90 samples per second when Normal mode is active. 180SPS with Turbo mode */         
    SPS_175,                /*!< 3 : 175 samples per second when Normal mode is active. 350SPS with Turbo mode */         
    SPS_330,                /*!< 4 : 330 samples per second when Normal mode is active. 660SPS with Turbo mode */         
    SPS_600,                /*!< 5 : 600 samples per second when Normal mode is active. 1200SPS with Turbo mode */         
    SPS_1000,               /*!< 6 : 1000 samples per second when Normal mode is active. 2000SPS with Turbo mode */         
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
    MONITOR_VREFP_VREFN = 0x0C, /*!< 12 : Input measuring (Vrefp - Vrefn)/4. (PGA bypassed) */
    MONITOR_AVDD_AVSS,          /*!< 13 : Input measuring (AVDD - AVSS)/4. (PGA bypassed) */
}ads112c04_monitor_mode_t;

typedef enum{
    SENSOR_GAIN_1   =   1,  /*<! (default) can be used with PGA_DISABLED */
    SENSOR_GAIN_2   =   2,  /*<! can be used with PGA_DISABLED */
    SENSOR_GAIN_4   =   4,  /*<! can be used with PGA_DISABLED */
    SENSOR_GAIN_8   =   8,
    SENSOR_GAIN_16  =  16,
    SENSOR_GAIN_32  =  32,
    SENSOR_GAIN_64  =  64,
    SENSOR_GAIN_128 = 128
}ads112c04_sensor_gain_t;

typedef enum{
    PGA_ENABLED,    /*<! 0 : (default) Programmmable Gain Amplifier enabled */
    PGA_DISABLED,   /*<! 1 : The PGA is always enabled for gain settings 8 to 128, reggardless of this setting */
    ALL_PGA_STATUS
}ads112c04_PGA_status_t;

typedef enum{
    CURRENT_0_uA,
    CURRENT_10_uA   =   10,
    CURRENT_50_uA   =   50,
    CURRENT_100_uA  =  100,
    CURRENT_250_uA  =  250,
    CURRENT_500_uA  =  500,
    CURRENT_1000_uA = 1000,
    CURRENT_1500_uA = 1500
}ads112c04_current_t;

typedef enum{
    IDAC1,
    IDAC2,
    TOTAL_CURRENT_SOURCES
}ads112c04_current_source_t;

typedef enum{
    CURRENT_DISABLED,
    CURRENT_TO_AIN0,
    CURRENT_TO_AIN1,
    CURRENT_TO_AIN2,
    CURRENT_TO_AIN3,
    CURRENT_TO_REFP,
    CURRENT_TO_REFN,
    TOTAL_CURRENT_OUTPUTS
}ads112c04_current_output_t;

#endif