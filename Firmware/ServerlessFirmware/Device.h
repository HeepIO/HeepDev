#include "AutoGeneratedInfo.h"

extern unsigned int firmwareVersion;

extern struct Control controlList [];
extern unsigned int numberOfControls;

extern unsigned int vertexPointerList[];
extern unsigned int numberOfVertices;

extern heepByte resetHeepNetwork;

void ClearControls();
void ClearVertices();

unsigned char isVertexEqual(struct Vertex_Byte* vertex1, struct Vertex_Byte* vertex2);

void AddVertexPointer(unsigned int pointer);

void RemoveVertexListEntry(unsigned int pointer);

int DeleteVertex(struct Vertex_Byte myVertex);

void FillVertexListFromMemory();

void SetDeviceName(char* deviceName);

void SetDeviceIcon(char deviceIcon);

int SetControlValueByID(unsigned char controlID, unsigned int value, unsigned int highValue, unsigned int lowValue, unsigned char setFromNetwork);

heepByte GetControlTypeFromControlID(heepByte controlID);

int SetControlValueByIDBuffer(unsigned char controlID, heepByte* buffer, int bufferStartPoint, int bufferLength, unsigned char setFromNetwork);

int SetControlValueByIDFromNetwork(unsigned char controlID, unsigned int value, unsigned int highValue, unsigned int lowValue);

int SetControlValueByIDFromNetworkBuffer(unsigned char controlID, heepByte* buffer, int bufferStartPoint, int bufferLength);
