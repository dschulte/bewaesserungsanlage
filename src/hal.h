#ifdef HARDWARE_VERSION_1_0
#define SENSOR_1_PIN 34
#define SENSOR_2_PIN 35
#define SENSOR_3_PIN 32
#define SENSOR_4_PIN 33
#define SENSOR_5_PIN 25
#define SENSOR_6_PIN 26

#define OUTPUT_1_PIN 27
#define OUTPUT_2_PIN 14
#define OUTPUT_3_PIN 12
#define OUTPUT_4_PIN 13
#define OUTPUT_5_PIN 19
#define OUTPUT_6_PIN 5
#else
#ifdef HARDWARE_VERSION_2_0
#define SENSOR_1_PIN 36
#define SENSOR_2_PIN 39
#define SENSOR_3_PIN 34
#define SENSOR_4_PIN 35
#define SENSOR_5_PIN 32
#define SENSOR_6_PIN 33

#define OUTPUT_1_PIN 25
#define OUTPUT_2_PIN 26
#define OUTPUT_3_PIN 27
#define OUTPUT_4_PIN 13
#define OUTPUT_5_PIN 19
#define OUTPUT_6_PIN 16
#else
#error Unknown or unset hardware version
#endif
#endif