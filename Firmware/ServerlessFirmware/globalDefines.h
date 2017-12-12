#pragma once

#define FIRMWARE_VERSION 1

#define FACTORY_RESET_PIN 4

#define MAX_MEMORY 255
#define NUM_VERTICES 100
#define NUM_CONTROLS 100
#define OUTPUT_BUFFER_SIZE 200
#define INPUT_BUFFER_SIZE 200
#define RESEND_ACK_BYTES 200

#define SYSTEM_TASK_INTERVAL 1000 // Time in ms
#define NUMBER_OF_TASKS 4

// Timeout is total timeout for packet.
// Packet will retry NUM_RETRIES time within ACK_TIMEOUT
// ACK_TIMEOUT is in 10s of milliseconds. So ACK_TIMEOUT of 10 is a 100ms timeout.
#define ACK_TIMEOUT 10 // In 10ms increments
#define NUM_RETRIES 3

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

#ifdef ON_PC
#include "Simulation_Timer.h"
#endif

#ifdef ON_ARDUINO
#include "Arduino_Timer.h"
#endif

#ifdef SIMULATION
#include "Simulation_Timer.h"
#endif

#ifdef ON_PIC
#include "PICW5500_Timer.h"
#endif