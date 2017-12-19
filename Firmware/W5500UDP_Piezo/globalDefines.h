#define FIRMWARE_VERSION 1

#define FACTORY_RESET_PIN 4

#define MAX_MEMORY 255
#define NUM_VERTICES 10
#define NUM_CONTROLS 10
#define OUTPUT_BUFFER_SIZE 100
#define INPUT_BUFFER_SIZE 100

#define SYSTEM_TASK_INTERVAL 1000 // Time in ms
#define NUMBER_OF_TASKS 4

//#define ON_ARDUINO
//#define ON_PC

#ifndef ON_ARDUINO
#include <string.h>
#endif

//#define USE_INDEXED_IDS

#define STANDARD_ID_SIZE 4

#ifdef USE_INDEXED_IDS
#define ID_SIZE 1
#else 
#define ID_SIZE STANDARD_ID_SIZE
#endif

typedef unsigned char heepByte;
