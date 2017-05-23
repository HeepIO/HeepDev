#define FIRMWARE_VERSION 1

#define MAX_MEMORY 255
#define NUM_VERTICES 100
#define NUM_CONTROLS 100
#define OUTPUT_BUFFER_SIZE 200
#define INPUT_BUFFER_SIZE 200

#define SYSTEM_TASK_INTERVAL 1000 // Time in ms
#define NUMBER_OF_TASKS 4

//#define ON_ARDUINO

//#define USE_INDEXED_IDS

#ifdef USE_INDEXED_IDS
#define ID_SIZE 1
#else 
#define ID_SIZE 4
#endif