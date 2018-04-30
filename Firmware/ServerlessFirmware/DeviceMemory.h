#pragma once

#include "AutoGeneratedInfo.h"

#ifdef DEVICE_USES_WIFI
	#define NUM_COPS_UNDERSTOOD 		10
#else
	#define NUM_COPS_UNDERSTOOD 		9
#endif

// OPCodes
#define ClientDataOpCode 		 	0x01
#define ControlOpCode 			 	0x02
#define VertexOpCode 			 	0x03
#define IconIDOpCode 			 	0x04
#define CustomIconDrawingOpCode 	0x05
#define DeviceNameOpCode 		 	0x06
#define FrontEndPositionOpCode	 	0x07
#define DeviceIPOpCode 			 	0x08
	
#define IsHeepDeviceOpCode 		 	0x09
#define SetValueOpCode 			 	0x0A
#define SetPositionOpCode 		 	0x0B
#define SetVertexOpCode 		 	0x0C
#define DeleteVertexOpCode 		 	0x0D
#define EchoOpCode 				 	0x0E

#define MemoryDumpOpCode 			0x0F
#define SuccessOpCode 				0x10
#define ErrorOpCode 				0x11

#define FragmentOpCode 				0x12

#define AddMOPOpCode 				0x13
#define DynamicMemorySizeOpCode 	0x14
#define DeleteMOPOpCode 			0x15
#define LocalDeviceIDOpCode 		0x16

#define AnalyticsOpCode				0x1F

#define WiFiSSIDOpCode				0x20
#define WiFiPasswordOpCode			0x21

#define SetWiFiDataOpCode			0x22
#define SetNameOpCode				0x23

#define ResetDeviceNetwork			0x24

#define USER_MOP_START_ID			0x50
#define USER_MOP_END_ID				0x5A

extern unsigned char deviceMemory[];
extern unsigned int curFilledMemory; // Indicate the curent filled memory. 
						 // Also serve as a place holder to 
						 // show the back of allocated memory
extern unsigned char memoryChanged;

extern unsigned char controlRegister;

int GetNumBytesToReadForMOP(unsigned int pointer);
heepByte GetMOPPointer(heepByte MOP, unsigned int *pointer, unsigned int *counter);

heepByte AddUserMOP(heepByte userMOPNumber, heepByte* buffer, int bufferLength, heepByte* deviceID);
heepByte GetUserMOP(heepByte userMOPNumber, heepByte* buffer, int* bytesReturned);

void PerformPreOpCodeProcessing_Byte(heepByte* deviceID);
void SetControlRegister();

unsigned int SkipOpCode(unsigned int counter);
void ClearDeviceMemory();

void AddNewCharToMemory(unsigned char newMem);

void AddBufferToMemory(heepByte* buffer, heepByte size);

void CreateBufferFromNumber(heepByte* buffer, unsigned long number, heepByte size);

void AddNumberToMemoryWithSpecifiedBytes(unsigned long number, int numBytes);

void AddDeviceIDToMemory_Byte(heepByte* deviceID);
void AddIndexOrDeviceIDToMemory_Byte(heepByte* deviceID);

void AddIPToMemory(struct HeepIPAddress theIP);

heepByte SetDeviceNameInMemory_Byte(char* deviceName, int numCharacters, heepByte* deviceID);
void SetIconIDInMemory_Byte(char iconID, heepByte* deviceID);

void SetIconDataInMemory_Byte(char* iconData, int numCharacters, heepByte* deviceID);

#ifdef USE_ANALYTICS

void SetAnalyticsDataControlValueInMemory_Byte(heepByte controlID, int controlValue, heepByte *deviceID);
int GetNextAnalyticsDataPointer(int startingPointer);
uint64_t GetTimeFromAnalyticsMOP(int MOPAddress);

#endif

unsigned int ParseXYOpCode_Byte(int *x, int *y, heepByte* deviceID, unsigned int counter);

unsigned int GetXYFromMemory_Byte(int *x, int *y, heepByte* deviceID, unsigned int* XYMemPosition);

void SetXYInMemory_Byte(int x, int y, heepByte* deviceID);

heepByte UpdateXYInMemory_Byte(int x, int y, heepByte* deviceID);

heepByte GetIPFromMemory(struct HeepIPAddress* theIP);
void SetIPInMemory_Byte(struct HeepIPAddress theIP, heepByte* deviceID);
void DeleteVertexAtPointer(unsigned long pointer);
int GetVertexAtPointer_Byte(unsigned long pointer, struct Vertex_Byte* returnedVertex);

heepByte SetVertexInMemory_Byte(struct Vertex_Byte theVertex, unsigned int* vertexPointer);

int GetNextVertexPointer(unsigned int* pointer,unsigned int* counter);

unsigned int GetFragmentFromMemory(int *pointerToFragment, int *numFragementBytes);
void RemoveUnusedBytesAtPointer(int pointer, int numBytes);
void DefragmentMemory();

heepByte DeleteWiFiSetting(int priority, heepByte* deviceID);

// Returns size of returned buffer
heepByte GetIndexedDeviceID_Byte(heepByte* deviceID);

heepByte GetDeviceIDFromIndex_Byte(heepByte* index, heepByte* returnedID);

heepByte WillMemoryOverflow(int numBytesToBeAdded);
void FragmentAllOfMOP(heepByte inputMOP);
void ImmediatelyClearAllOfMOP(heepByte inputMOP);
