#include "unity.h"
#include "mock_ads112c04_hal.h"
#include "ads112c04_utilities.h"

//Only the first 14bits MSB are used.
void test_utilities_raw_to_degree_14_MSB_used (void)
{
    //given
    int16_t raw_data = 0x0003;
    //expect
    TEST_ASSERT_EQUAL_FLOAT(0, ads112c04_rawTemperatureToCelsius(raw_data));
}

//The raw value 0x1000 is 128 degrees
void test_utilities_raw_to_degree_128 (void)
{
    //given
    int16_t raw_data = 0x1000 << 2;
    //expect
    TEST_ASSERT_EQUAL_FLOAT(128, ads112c04_rawTemperatureToCelsius(raw_data));
}

//The raw value 0x0001 is  0.03125 degrees
void test_utilities_raw_to_degree_0_03125 (void)
{
    //given
    int16_t raw_data = 0x0001 << 2;
    //expect
    TEST_ASSERT_EQUAL_FLOAT(0.03125, ads112c04_rawTemperatureToCelsius(raw_data));    
}

//The raw value 0x0FFF is  127.96875
void test_utilities_raw_to_degree_127_06875 (void)
{
    //given
    int16_t raw_data = 0x0FFF << 2;
    //expect
    TEST_ASSERT_EQUAL_FLOAT(127.96875, ads112c04_rawTemperatureToCelsius(raw_data));      
}

//The raw value 0x3FF8 is -0.25
void test_utilities_raw_to_degree_less_0_25 (void)
{
    //given
    int16_t raw_data = 0x3FF8 << 2;
    //expect
    TEST_ASSERT_EQUAL_FLOAT(-0.25, ads112c04_rawTemperatureToCelsius(raw_data));     
}
