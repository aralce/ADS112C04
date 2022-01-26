#ifndef ADS112CO4_CORE_DEFINES_
#define ADS112CO4_CORE_DEFINES_


#define SENSOR_INIT_DELAY_MS  500

//SENSOR COMANDS
#define COMMAND_RESET           0x06
#define COMMAND_READ_DATA       0x10
#define COMMAND_READ_REGISTER   0x20 
#define COMMAND_WRITE_REGISTER  0x40

//CONFIG REGISTERS TO MERGE WITH REGISTER COMMMANDS
#define CONFIG_REGISTER_1_CM    0x04  //  01xx


//CONFIG1 MASKS
#define CONVERSION_MODE_MASK    0x03
#define OPERATION_MODE_MASK     0x04

#endif