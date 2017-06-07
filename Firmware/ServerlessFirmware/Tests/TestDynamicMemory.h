#include "UnitTestSystem.h"
#include "../ActionAndResponseOpCodes.h"

void PrintDeviceMemory()
{
	cout << "Start" << endl;

	for(int i = 0; i < curFilledMemory; i++)
	{
		cout << (int)deviceMemory[i] << " ";
	}
	cout << endl;
}

void CreateFakeDeviceID(heepByte* deviceID)
{
	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		deviceID[i] = i + 1;
	}
}

int GetMemCounterStart()
{
#ifdef USE_INDEXED_IDS
	return 1 + ID_SIZE + 1 + STANDARD_ID_SIZE;
#else
	return 0;
#endif
}

void TestAddCharToBuffer()
{
	std::string TestName = "Add Char to Buffer";

	unsigned char buffer[5];
	int tracker = 0;
	tracker = AddCharToBuffer(buffer, tracker, '2');
	tracker = AddCharToBuffer(buffer, tracker, '5');

	ExpectedValue valueList [2];
	valueList[0].valueName = "Buffered Char 1";
	valueList[0].expectedValue = '2';
	valueList[0].actualValue = buffer[0];

	valueList[1].valueName = "Buffered Char 2";
	valueList[1].expectedValue = '5';
	valueList[1].actualValue = buffer[1];

	CheckResults(TestName, valueList, 2);
}

void TestAddCharToDeviceMemory()
{
	std::string TestName = "Add Char to Device Memory";

	AddNewCharToMemory('3');
	AddNewCharToMemory('6');

	ExpectedValue valueList [2];
	valueList[0].valueName = "Buffered Char 1";
	valueList[0].expectedValue = '3';
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Buffered Char 2";
	valueList[1].expectedValue = '6';
	valueList[1].actualValue = deviceMemory[1];

	CheckResults(TestName, valueList, 2);
}

void TestClearDeviceMemory()
{
	std::string TestName = "Clear Device Memory";

	ClearDeviceMemory();

	AddNewCharToMemory('d');
	AddNewCharToMemory('3');

	ExpectedValue valueList [2];
	valueList[0].valueName = "Memory Size Before Clear";
	valueList[0].expectedValue = 2;
	valueList[0].actualValue = curFilledMemory;

	ClearDeviceMemory();

	valueList[1].valueName = "Memory Size After Clear";
	valueList[1].expectedValue = 0;
	valueList[1].actualValue = curFilledMemory;

	CheckResults(TestName, valueList, 2);
}

void TestAddDeviceIDToMemory()
{
	std::string TestName = "Add Device ID to Device Memory";

	unsigned long ID = 0x09080706;

	ClearDeviceMemory();
	AddDeviceIDToMemory(ID);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Octet 4";
	valueList[0].expectedValue = 9;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Octet 3";
	valueList[1].expectedValue = 8;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Octet 2";
	valueList[2].expectedValue =7;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Octet 1";
	valueList[3].expectedValue = 6;
	valueList[3].actualValue = deviceMemory[3];

	CheckResults(TestName, valueList, 4);
}

void TestAddDeviceIDToBuffer()
{
	std::string TestName = "Add Device ID to Buffer";

	unsigned long ID = 0x09080706;

	unsigned char buffer[5];
	int tracker = 0;
	tracker = AddDeviceIDToBuffer(buffer, tracker, ID);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Octet 4";
	valueList[0].expectedValue = 9;
	valueList[0].actualValue = buffer[0];

	valueList[1].valueName = "Octet 3";
	valueList[1].expectedValue = 8;
	valueList[1].actualValue = buffer[1];

	valueList[2].valueName = "Octet 2";
	valueList[2].expectedValue =7;
	valueList[2].actualValue = buffer[2];

	valueList[3].valueName = "Octet 1";
	valueList[3].expectedValue = 6;
	valueList[3].actualValue = buffer[3];

	CheckResults(TestName, valueList, 4);
}

void TestAddIPToDeviceMemory()
{
	std::string TestName = "Add Device IP to Memory";

	ClearDeviceMemory();
	HeepIPAddress myIP;
	myIP.Octet4 = 192;
	myIP.Octet3 = 100;
	myIP.Octet2 = 1;
	myIP.Octet1 = 100;
	AddIPToMemory(myIP);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Octet 4";
	valueList[0].expectedValue = 192;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Octet 3";
	valueList[1].expectedValue = 100;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Octet 2";
	valueList[2].expectedValue =1;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Octet 1";
	valueList[3].expectedValue = 100;
	valueList[3].actualValue = deviceMemory[3];

	CheckResults(TestName, valueList, 4);
}

void TestDeviceNameOpCode()
{
	std::string TestName = "Device Name Op Code";

	ClearDeviceMemory();
	char* deviceName = "Jacob";
	SetDeviceNameInMemory(deviceName, strlen(deviceName), 0x01020304);

	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [7];
	valueList[0].valueName = "Device Name OpCode";
	valueList[0].expectedValue = DeviceNameOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 5;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Letter One";
	valueList[2].expectedValue = 'J';
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	valueList[3].valueName = "Letter Two";
	valueList[3].expectedValue = 'a';
	valueList[3].actualValue = deviceMemory[memCheckStart + ID_SIZE + 3];

	valueList[4].valueName = "Letter Three";
	valueList[4].expectedValue = 'c';
	valueList[4].actualValue = deviceMemory[memCheckStart + ID_SIZE + 4];

	valueList[5].valueName = "Letter Four";
	valueList[5].expectedValue = 'o';
	valueList[5].actualValue = deviceMemory[memCheckStart + ID_SIZE + 5];

	valueList[6].valueName = "Letter Five";
	valueList[6].expectedValue = 'b';
	valueList[6].actualValue = deviceMemory[memCheckStart + ID_SIZE + 6];

	CheckResults(TestName, valueList, 7);

	PrintDeviceMemory();
}

void TestIconIDOpCode()
{
	std::string TestName = "Icon ID Op Code";

	ClearDeviceMemory();
	SetIconIDInMemory(4, 0x01020304);

	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [3];
	valueList[0].valueName = "Icon ID OpCode";
	valueList[0].expectedValue = IconIDOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Letter One";
	valueList[2].expectedValue = 4;
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	CheckResults(TestName, valueList, 3);
}

void TestIconDataOpCode()
{
	std::string TestName = "Icon Data Op Code";

	ClearDeviceMemory();
	char* deviceName = "Jacob";
	SetIconDataInMemory(deviceName, strlen(deviceName), 0x01020304);

	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [7];
	valueList[0].valueName = "Icon Data OpCode";
	valueList[0].expectedValue = CustomIconDrawingOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 5;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Letter One";
	valueList[2].expectedValue = 'J';
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	valueList[3].valueName = "Letter Two";
	valueList[3].expectedValue = 'a';
	valueList[3].actualValue = deviceMemory[memCheckStart + ID_SIZE + 3];

	valueList[4].valueName = "Letter Three";
	valueList[4].expectedValue = 'c';
	valueList[4].actualValue = deviceMemory[memCheckStart + ID_SIZE + 4];

	valueList[5].valueName = "Letter Four";
	valueList[5].expectedValue = 'o';
	valueList[5].actualValue = deviceMemory[memCheckStart + ID_SIZE + 5];

	valueList[6].valueName = "Letter Five";
	valueList[6].expectedValue = 'b';
	valueList[6].actualValue = deviceMemory[memCheckStart + ID_SIZE + 6];

	CheckResults(TestName, valueList, 7);
}

void TestSetXYOpCode()
{
	std::string TestName = "Front End XY Op Code";

	ClearDeviceMemory();
	SetXYInMemory(312, 513, 0x01020304);
	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [6];
	valueList[0].valueName = "Front End XY OpCode";
	valueList[0].expectedValue = FrontEndPositionOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 4;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "X High Byte";
	valueList[2].expectedValue = 312 >> 8;
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	valueList[3].valueName = "X Low Byte";
	valueList[3].expectedValue = 312%256;
	valueList[3].actualValue = deviceMemory[memCheckStart + ID_SIZE + 3];

	valueList[4].valueName = "Y High Byte";
	valueList[4].expectedValue = 513 >> 8;
	valueList[4].actualValue = deviceMemory[memCheckStart + ID_SIZE + 4];

	valueList[5].valueName = "Y Low Byte";
	valueList[5].expectedValue = 513%256;
	valueList[5].actualValue = deviceMemory[memCheckStart + ID_SIZE + 5];

	CheckResults(TestName, valueList, 6);
}

void TestSetIPOpCode()
{
	std::string TestName = "Set IP OpCode";

	ClearDeviceMemory();
	HeepIPAddress myIP;
	myIP.Octet4 = 192;
	myIP.Octet3 = 168;
	myIP.Octet2 = 1;
	myIP.Octet1 = 100;
	SetIPInMemory(myIP, 0x01020304);

	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [6];
	valueList[0].valueName = "IP OpCode";
	valueList[0].expectedValue = DeviceIPOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 4;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Octet 4";
	valueList[2].expectedValue = 192;
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	valueList[3].valueName = "Octet 3";
	valueList[3].expectedValue = 168;
	valueList[3].actualValue = deviceMemory[memCheckStart + ID_SIZE + 3];

	valueList[4].valueName = "Octet 2";
	valueList[4].expectedValue = 1;
	valueList[4].actualValue = deviceMemory[memCheckStart + ID_SIZE + 4];

	valueList[5].valueName = "Octet 1";
	valueList[5].expectedValue = 100;
	valueList[5].actualValue = deviceMemory[memCheckStart + ID_SIZE + 5];

	CheckResults(TestName, valueList, 6);
}

void TestSetVertexOpCode()
{
	std::string TestName = "Set Vertex OpCode";

	ClearDeviceMemory();
	Vertex myVertex;
	HeepIPAddress myIP;
	myIP.Octet4 = 192;
	myIP.Octet3 = 168;
	myIP.Octet2 = 1;
	myIP.Octet1 = 100;
	myVertex.rxIPAddress = myIP;
	myVertex.txID = 0x01020304;
	myVertex.rxID = 0x09080706;
	myVertex.txControlID = 0x01;
	myVertex.rxControlID = 0x02;
	SetVertexInMemory(myVertex);

	int memCheckStart = GetMemCounterStart()*2;

	ExpectedValue valueList [8];
	valueList[0].valueName = "Vertex OpCode";
	valueList[0].expectedValue = VertexOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = ID_SIZE + 6;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Tx Control ID";
	valueList[2].expectedValue = 1;
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + ID_SIZE + 2];

	valueList[3].valueName = "Rx Control ID";
	valueList[3].expectedValue = 2;
	valueList[3].actualValue = deviceMemory[memCheckStart + ID_SIZE + ID_SIZE + 3];

	valueList[4].valueName = "IP Octet 4";
	valueList[4].expectedValue = 192;
	valueList[4].actualValue = deviceMemory[memCheckStart + ID_SIZE + ID_SIZE + 4];

	valueList[5].valueName = "IP Octet 3";
	valueList[5].expectedValue = 168;
	valueList[5].actualValue = deviceMemory[memCheckStart + ID_SIZE + ID_SIZE + 5];

	valueList[6].valueName = "IP Octet 2";
	valueList[6].expectedValue = 1;
	valueList[6].actualValue = deviceMemory[memCheckStart + ID_SIZE + ID_SIZE + 6];

	valueList[7].valueName = "IP Octet 1";
	valueList[7].expectedValue = 100;
	valueList[7].actualValue = deviceMemory[memCheckStart + ID_SIZE + ID_SIZE + 7];

	CheckResults(TestName, valueList, 8);
}

void TestGetVertex()
{
	std::string TestName = "Get Vertex From Memory";

	ClearDeviceMemory();
	Vertex myVertex;
	HeepIPAddress myIP;
	myIP.Octet4 = 192;
	myIP.Octet3 = 168;
	myIP.Octet2 = 1;
	myIP.Octet1 = 100;
	myVertex.rxIPAddress = myIP;
	myVertex.txID = 0x01020304;
	myVertex.rxID = 0x09080706;
	myVertex.txControlID = 0x01;
	myVertex.rxControlID = 0x02;
	SetVertexInMemory(myVertex);

	int memCheckStart = GetMemCounterStart()*2;

	Vertex newVertex;
	int success = GetVertexAtPonter(memCheckStart, newVertex);

	ExpectedValue valueList [8];
	valueList[0].valueName = "TXID";
	valueList[0].expectedValue = myVertex.txID;
	valueList[0].actualValue = newVertex.txID;

	valueList[1].valueName = "RXID";
	valueList[1].expectedValue = myVertex.rxID;
	valueList[1].actualValue = newVertex.rxID;

	valueList[2].valueName = "TX Control ID";
	valueList[2].expectedValue = myVertex.txControlID;
	valueList[2].actualValue = newVertex.txControlID;

	valueList[3].valueName = "RX Control ID";
	valueList[3].expectedValue = myVertex.rxControlID;
	valueList[3].actualValue = newVertex.rxControlID;

	valueList[4].valueName = "IP 4";
	valueList[4].expectedValue = myVertex.rxIPAddress.Octet4;
	valueList[4].actualValue = newVertex.rxIPAddress.Octet4;

	valueList[5].valueName = "IP 3";
	valueList[5].expectedValue = myVertex.rxIPAddress.Octet3;
	valueList[5].actualValue = newVertex.rxIPAddress.Octet3;

	valueList[6].valueName = "IP 2";
	valueList[6].expectedValue = myVertex.rxIPAddress.Octet2;
	valueList[6].actualValue = newVertex.rxIPAddress.Octet2;

	valueList[7].valueName = "IP 1";
	valueList[7].expectedValue = myVertex.rxIPAddress.Octet1;
	valueList[7].actualValue = newVertex.rxIPAddress.Octet1;

	CheckResults(TestName, valueList, 8);
}

void TestGetXYFromMemory()
{
	std::string TestName = "Get Front End XY";

	ClearDeviceMemory();
	SetXYInMemory(312, 513, 0x01020304);

	int x = 0; int y = 0; unsigned long deviceID = 0x01020304; unsigned int xyMemPosition = 0; 
	int retVal = GetXYFromMemory(x, y, deviceID, xyMemPosition);

	ExpectedValue valueList [3];
	valueList[0].valueName = "X Position";
	valueList[0].expectedValue = 312;
	valueList[0].actualValue = x;

	valueList[1].valueName = "Y Position";
	valueList[1].expectedValue = 513;
	valueList[1].actualValue = y;

	valueList[2].valueName = "Success Return";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = retVal;

	CheckResults(TestName, valueList, 3);
}

void TestSkipOpCode()
{
	std::string TestName = "Skip Op Code";

	ClearDeviceMemory();
	HeepIPAddress myIP;
	myIP.Octet4 = 192;
	myIP.Octet3 = 168;
	myIP.Octet2 = 1;
	myIP.Octet1 = 100;
	SetIPInMemory(myIP, 0x01020304);
	char* deviceName = "Jacob";
	SetDeviceNameInMemory(deviceName, strlen(deviceName), 0x01020304);
	SetXYInMemory(312, 513, 0x01020304);

	int x = 0; int y = 0; unsigned long deviceID = 0x01020304; unsigned int xyMemPosition = 0; 
	int retVal = GetXYFromMemory(x, y, deviceID, xyMemPosition);

	ExpectedValue valueList [3];
	valueList[0].valueName = "X Position";
	valueList[0].expectedValue = 312;
	valueList[0].actualValue = x;

	valueList[1].valueName = "Y Position";
	valueList[1].expectedValue = 513;
	valueList[1].actualValue = y;

	valueList[2].valueName = "Success Return";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = retVal;

	CheckResults(TestName, valueList, 3);
}

void TestUpdateXYPosition()
{
	std::string TestName = "Test Update XY Position";

	ClearDeviceMemory();
	UpdateXYInMemory(312, 513, 0x01020304);

	int x = 0; int y = 0; unsigned long deviceID = 0x01020304; unsigned int xyMemPosition = 0; 
	int retVal = GetXYFromMemory(x, y, deviceID, xyMemPosition);

	ExpectedValue valueList [6];
	valueList[0].valueName = "X Position";
	valueList[0].expectedValue = 312;
	valueList[0].actualValue = x;

	valueList[1].valueName = "Y Position";
	valueList[1].expectedValue = 513;
	valueList[1].actualValue = y;

	valueList[2].valueName = "Success Return";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = retVal;

	UpdateXYInMemory(16235, 256, 0x01020304);
	retVal = GetXYFromMemory(x, y, deviceID, xyMemPosition);

	valueList[3].valueName = "X Position";
	valueList[3].expectedValue = 16235;
	valueList[3].actualValue = x;

	valueList[4].valueName = "Y Position";
	valueList[4].expectedValue = 256;
	valueList[4].actualValue = y;

	valueList[5].valueName = "Success Return";
	valueList[5].expectedValue = 0;
	valueList[5].actualValue = retVal;

	CheckResults(TestName, valueList, 6);
}

void TestDefragmentDeviceMemory()
{
	std::string TestName = "Test Defragmented Vertex";

	ClearDeviceMemory();

	Vertex theVertex;
	theVertex.rxID = 0x01020304;
	theVertex.txID = 0x05060708;
	theVertex.rxControlID = 1;
	theVertex.txControlID = 2;
	HeepIPAddress theIP;
	theIP.Octet4 = 192;
	theIP.Octet3 = 168;
	theIP.Octet2 = 1;
	theIP.Octet1 = 150;
	theVertex.rxIPAddress = theIP;

	int memCheckStart = GetMemCounterStart()*2;

	SetVertexInMemory(theVertex);
	ExpectedValue valueList [3];
	unsigned int beforeDeletionMemory = curFilledMemory;
	valueList[0].valueName = "Memory Filled Before Deletion";
	valueList[0].expectedValue = ID_SIZE+2+ID_SIZE+6 + memCheckStart;
	valueList[0].actualValue = beforeDeletionMemory;

	DeleteVertexAtPointer(memCheckStart);
	unsigned int afterDeletionMemory = curFilledMemory;
	valueList[1].valueName = "Memory Filled After Deletion";
	valueList[1].expectedValue = ID_SIZE+2+ID_SIZE+6 + memCheckStart;
	valueList[1].actualValue = afterDeletionMemory;

	DefragmentMemory();
	valueList[2].valueName = "Memory Filled after Defragmentation";
	valueList[2].expectedValue = memCheckStart;
	valueList[2].actualValue = curFilledMemory;
	CheckResults(TestName, valueList, 3);
}

void TestDefragmentDeviceMemoryAtEnd()
{
	std::string TestName = "Test Defragmented Vertex at End";

	ClearDeviceMemory();

	Vertex theVertex;
	theVertex.rxID = 0x01020304;
	theVertex.txID = 0x05060708;
	theVertex.rxControlID = 1;
	theVertex.txControlID = 2;
	HeepIPAddress theIP;
	theIP.Octet4 = 192;
	theIP.Octet3 = 168;
	theIP.Octet2 = 1;
	theIP.Octet1 = 150;
	theVertex.rxIPAddress = theIP;


	SetDeviceNameInMemory("Crowbar", 7, 0x01020304);
	SetIPInMemory(theIP, 0x04030210);

	int vertexPointer = curFilledMemory;

#ifdef USE_INDEXED_IDS
	vertexPointer += ID_SIZE+STANDARD_ID_SIZE+2;
#endif

	SetVertexInMemory(theVertex);
	unsigned int beforeDeletionMemory = curFilledMemory;
	DeleteVertexAtPointer(vertexPointer);
	unsigned int afterDeletionMemory = curFilledMemory;

	ExpectedValue valueList [2];
	
	valueList[0].valueName = "Memory Filled Before and after Deletion";
	valueList[0].expectedValue = afterDeletionMemory;
	valueList[0].actualValue = beforeDeletionMemory;

	DefragmentMemory();
	valueList[1].valueName = "Memory Filled after Defragmentation";
	valueList[1].expectedValue = afterDeletionMemory - (ID_SIZE + 2 + ID_SIZE + 6);
	valueList[1].actualValue = curFilledMemory;

	CheckResults(TestName, valueList, 2);
}

void TestDefragmentDeviceMemoryDeviceAtFront()
{
	std::string TestName = "Test Defragmented Vertex at Front";

	ClearDeviceMemory();

	Vertex theVertex;
	theVertex.rxID = 0x01020304;
	theVertex.txID = 0x05060708;
	theVertex.rxControlID = 1;
	theVertex.txControlID = 2;
	HeepIPAddress theIP;
	theIP.Octet4 = 192;
	theIP.Octet3 = 168;
	theIP.Octet2 = 1;
	theIP.Octet1 = 150;
	theVertex.rxIPAddress = theIP;

	int memCheckStart = GetMemCounterStart()*2;

	SetVertexInMemory(theVertex);
	SetDeviceNameInMemory("Crowbar", 7, 0x01020304);
	SetIPInMemory(theIP, 0x04030210);
	ExpectedValue valueList [2];
	unsigned int beforeDeletionMemory = curFilledMemory;
	DeleteVertexAtPointer(memCheckStart);
	unsigned int afterDeletionMemory = curFilledMemory;

	valueList[0].valueName = "Memory Filled Before and after Deletion";
	valueList[0].expectedValue = afterDeletionMemory;
	valueList[0].actualValue = beforeDeletionMemory;

	DefragmentMemory();
	valueList[1].valueName = "Memory Filled after Defragmentation";
	valueList[1].expectedValue = afterDeletionMemory - (ID_SIZE + 2 + ID_SIZE + 6);
	valueList[1].actualValue = curFilledMemory;

	CheckResults(TestName, valueList, 2);
}

void TestDefragmentDeviceMemoryInMiddle()
{
	std::string TestName = "Test Defragmented Vertex in Middle";

	ClearDeviceMemory();

	Vertex theVertex;
	theVertex.rxID = 0x01020304;
	theVertex.txID = 0x05060708;
	theVertex.rxControlID = 1;
	theVertex.txControlID = 2;
	HeepIPAddress theIP;
	theIP.Octet4 = 192;
	theIP.Octet3 = 168;
	theIP.Octet2 = 1;
	theIP.Octet1 = 150;
	theVertex.rxIPAddress = theIP;


	SetDeviceNameInMemory("Crowbar", 7, 0x01020304);

	int vertexPointer = curFilledMemory;

#ifdef USE_INDEXED_IDS
	vertexPointer += ID_SIZE+STANDARD_ID_SIZE+2;
#endif

	SetVertexInMemory(theVertex);
	SetIPInMemory(theIP, 0x04030210);
	unsigned int beforeDeletionMemory = curFilledMemory;
	DeleteVertexAtPointer(vertexPointer);
	unsigned int afterDeletionMemory = curFilledMemory;

	ExpectedValue valueList [2];
	valueList[0].valueName = "Memory Filled Before and after Deletion";
	valueList[0].expectedValue = afterDeletionMemory;
	valueList[0].actualValue = beforeDeletionMemory;

	DefragmentMemory();
	valueList[1].valueName = "Memory Filled after Defragmentation";
	valueList[1].expectedValue = afterDeletionMemory - (ID_SIZE + 2 + ID_SIZE + 6);
	valueList[1].actualValue = curFilledMemory;
	CheckResults(TestName, valueList, 2);
}

void TestBuildVertexListFromPointers()
{
	std::string TestName = "Test Build Vertex List";

	ClearDeviceMemory();

	Vertex theVertex;
	theVertex.rxID = 0x01020304;
	theVertex.txID = 0x05060708;
	theVertex.rxControlID = 1;
	theVertex.txControlID = 2;
	HeepIPAddress theIP;
	theIP.Octet4 = 192;
	theIP.Octet3 = 168;
	theIP.Octet2 = 1;
	theIP.Octet1 = 150;
	theVertex.rxIPAddress = theIP;

	Vertex theVertex2;
	theVertex2.rxID = 0x01020304;
	theVertex2.txID = 0x05060708;
	theVertex2.rxControlID = 1;
	theVertex2.txControlID = 2;
	theVertex2.rxIPAddress = theIP;


	SetDeviceNameInMemory("Crowbar", 7, 0x01020304);
	SetVertexInMemory(theVertex);
	SetIPInMemory(theIP, 0x04030210);
	SetVertexInMemory(theVertex2);

	FillVertexListFromMemory();

	ExpectedValue valueList [3];
	valueList[0].valueName = "Num Vertices";
	valueList[0].expectedValue = 2;
	valueList[0].actualValue = numberOfVertices;

	valueList[1].valueName = "Vertex 1 OpCode Check";
	valueList[1].expectedValue = 0x03;
	valueList[1].actualValue = deviceMemory[vertexPointerList[0]];

	valueList[2].valueName = "Vertex 2 OpCode Check";
	valueList[2].expectedValue = 0x03;
	valueList[2].actualValue = deviceMemory[vertexPointerList[1]];

	CheckResults(TestName, valueList, 3);
}	

void TestControlRegister()
{
	std::string TestName = "Test Control Register";

	SetControlRegister();

#ifdef USE_INDEXED_IDS

	ExpectedValue valueList [1];
	valueList[0].valueName = "Control Register Value";
	valueList[0].expectedValue = 0x04;
	valueList[0].actualValue = controlRegister;

#else 

	ExpectedValue valueList [1];
	valueList[0].valueName = "Control Register Value";
	valueList[0].expectedValue = 0x00;
	valueList[0].actualValue = controlRegister;

#endif

	CheckResults(TestName, valueList, 1);
}

void TestGetIndexFromDeviceID()
{
	std::string TestName = "Test Index From Device ID";

	ClearDeviceMemory();
	unsigned long indexId0 = GetIndexedDeviceID(0x01020304);
	unsigned long indexId1 = GetIndexedDeviceID(0x02304012);
	unsigned long indexID2 = GetIndexedDeviceID(0x01020304);
	unsigned long indexId3 = GetIndexedDeviceID(0x02395813);
	unsigned long indexId4 = GetIndexedDeviceID(0x02304012);

#ifdef USE_INDEXED_IDS

	ExpectedValue valueList [5];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 0;
	valueList[0].actualValue = indexId0;

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = indexId1;

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = indexID2;

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = 2;
	valueList[3].actualValue = indexId3;

	valueList[4].valueName = "Index Value 5";
	valueList[4].expectedValue = 1;
	valueList[4].actualValue = indexId4;

	CheckResults(TestName, valueList, 5);

#else 

	ExpectedValue valueList [5];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 0x01020304;
	valueList[0].actualValue = indexId0;

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 0x02304012;
	valueList[1].actualValue = indexId1;

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 0x01020304;
	valueList[2].actualValue = indexID2;

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = 0x02395813;
	valueList[3].actualValue = indexId3;

	valueList[4].valueName = "Index Value 5";
	valueList[4].expectedValue = 0x02304012;
	valueList[4].actualValue = indexId4;

	CheckResults(TestName, valueList, 5);

#endif
}

void TestGetDeviceIDFromIndex()
{
	std::string TestName = "Test Device ID From Index";

	ClearDeviceMemory();
	GetIndexedDeviceID(0x01020304);
	GetIndexedDeviceID(0x02304012);
	GetIndexedDeviceID(0x01020304);
	GetIndexedDeviceID(0x02395813);
	GetIndexedDeviceID(0x02304012);

	unsigned long realID1 = GetDeviceIDFromIndex(0);
	unsigned long realID2 = GetDeviceIDFromIndex(1);
	unsigned long realID3 = GetDeviceIDFromIndex(2);

#ifdef USE_INDEXED_IDS

	ExpectedValue valueList [3];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 0x01020304;
	valueList[0].actualValue = realID1;

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 0x02304012;
	valueList[1].actualValue = realID2;

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 0x02395813;
	valueList[2].actualValue = realID3;

	CheckResults(TestName, valueList, 3);

#else 

	ExpectedValue valueList [3];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 0;
	valueList[0].actualValue = realID1;

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = realID2;

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = realID3;

	CheckResults(TestName, valueList, 3);

#endif
}

void TestByteWiseDeviceEquality()
{
	std::string TestName = "Byte Wise device ID equality";

	heepByte ID1 [STANDARD_ID_SIZE];
	heepByte ID2 [STANDARD_ID_SIZE];

	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		ID1[i] = i;
		ID2[i] = i;
	}

	int devicesEqual = CheckDeviceIDEquality(ID1, ID2);

	ExpectedValue valueList [1];
	valueList[0].valueName = "Devices Equal";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = devicesEqual;

	CheckResults(TestName, valueList, 1);
}

void TestByteWiseDeviceInequality()
{
	std::string TestName = "Byte Wise device ID inequality";

	heepByte ID1 [STANDARD_ID_SIZE];
	heepByte ID2 [STANDARD_ID_SIZE];

	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		ID1[i] = i;
		ID2[i] = i;
	}

	ID2[2] = 8;

	int devicesEqual = CheckDeviceIDEquality(ID1, ID2);

	ExpectedValue valueList [1];
	valueList[0].valueName = "Devices Equal";
	valueList[0].expectedValue = 0;
	valueList[0].actualValue = devicesEqual;

	CheckResults(TestName, valueList, 1);
}

void TestGetDeviceIDFromMemory()
{
	std::string TestName = "Get Device ID From Memory";

	heepByte ID1 [STANDARD_ID_SIZE];
	heepByte ID2 [STANDARD_ID_SIZE];

	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		ID1[i] = i;
	}

	ClearDeviceMemory();
	curFilledMemory = AddDeviceIDToBuffer_Byte(deviceMemory, ID1, 0);

	int devicesEqual = CheckDeviceIDEquality(ID1, ID2);

	ExpectedValue valueList [1];
	valueList[0].valueName = "Devices Equal";
	valueList[0].expectedValue = 0;
	valueList[0].actualValue = devicesEqual;

	CheckResults(TestName, valueList, 1);
}

void TestGetDeviceIndexByteWise()
{
	std::string TestName = "Test Index From Device ID Bytewise";

	heepByte myID0 [STANDARD_ID_SIZE];
	heepByte myID1 [STANDARD_ID_SIZE];
	heepByte myID2 [STANDARD_ID_SIZE];
	heepByte myID3 [STANDARD_ID_SIZE];
	heepByte myID4 [STANDARD_ID_SIZE];

	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		myID0[i] = i + 1;
		myID1[i] = i + 3;
		myID2[i] = i + 1;
		myID3[i] = i + 9;
		myID4[i] = i + 14;
	}

	ClearDeviceMemory();

	heepByte retSize0 = GetIndexedDeviceID_Byte(myID0);
	heepByte retSize1 = GetIndexedDeviceID_Byte(myID1);
	heepByte retSize2 = GetIndexedDeviceID_Byte(myID2);
	heepByte retSize3 = GetIndexedDeviceID_Byte(myID3);
	heepByte retSize4 = GetIndexedDeviceID_Byte(myID4);

	PrintDeviceMemory();

#ifdef USE_INDEXED_IDS

	unsigned int startCount = 0;
	unsigned long value0 = GetNumberFromBuffer(myID0, startCount, retSize0);
	startCount = 0;
	unsigned long value1 = GetNumberFromBuffer(myID1, startCount, retSize1);
	startCount = 0;
	unsigned long value2 = GetNumberFromBuffer(myID2, startCount, retSize2);
	startCount = 0;
	unsigned long value3 = GetNumberFromBuffer(myID3, startCount, retSize3);
	startCount = 0;
	unsigned long value4 = GetNumberFromBuffer(myID4, startCount, retSize4);

	ExpectedValue valueList [5];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 0;
	valueList[0].actualValue = value0;

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = value1;

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = value2;

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = 2;
	valueList[3].actualValue = value3;

	valueList[4].valueName = "Index Value 5";
	valueList[4].expectedValue = 3;
	valueList[4].actualValue = value4;

	CheckResults(TestName, valueList, 5);

#else 

	ExpectedValue valueList [5];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = retSize0;
	valueList[0].actualValue = STANDARD_ID_SIZE;

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = retSize1;
	valueList[1].actualValue = STANDARD_ID_SIZE;

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = retSize2;
	valueList[2].actualValue = STANDARD_ID_SIZE;

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = retSize3;
	valueList[3].actualValue = STANDARD_ID_SIZE;

	valueList[4].valueName = "Index Value 5";
	valueList[4].expectedValue = retSize4;
	valueList[4].actualValue = STANDARD_ID_SIZE;

	CheckResults(TestName, valueList, 5);

#endif
}

void TestCreateBufferFromNumber()
{
	std::string TestName = "Test Create Buffer From Number";

	heepByte myBuf [4];

	CreateBufferFromNumber(myBuf, 0x01020304, 4);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = myBuf[0];

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 2;
	valueList[1].actualValue = myBuf[1];

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 3;
	valueList[2].actualValue = myBuf[2];

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = 4;
	valueList[3].actualValue = myBuf[3];

	CheckResults(TestName, valueList, 4);
}

void TestAddBufferToBuffer()
{
	std::string TestName = "Test Add Buffer to Buffer";

	heepByte myBuf [4];
	heepByte theirBuf [4];
	for(int i = 0; i < 4; i++)
	{
		myBuf[i] = i + 3;
	}

	unsigned int rxCounter = 0;
	unsigned int txCounter = 0;
	AddBufferToBuffer(theirBuf, myBuf, 4, rxCounter, txCounter);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = theirBuf[0];
	valueList[0].actualValue = myBuf[0];

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = theirBuf[1];
	valueList[1].actualValue = myBuf[1];

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = theirBuf[2];
	valueList[2].actualValue = myBuf[2];

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = theirBuf[3];
	valueList[3].actualValue = myBuf[3];

	CheckResults(TestName, valueList, 4);
}

void TestAddBufferToMemory()
{
	std::string TestName = "Test Add Buffer to Memory";

	heepByte myBuf [4];
	for(int i = 0; i < 4; i++)
	{
		myBuf[i] = i + 3;
	}

	ClearDeviceMemory();
	AddBufferToMemory(myBuf, 4);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = deviceMemory[0];
	valueList[0].actualValue = myBuf[0];

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = deviceMemory[1];
	valueList[1].actualValue = myBuf[1];

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = deviceMemory[2];
	valueList[2].actualValue = myBuf[2];

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = deviceMemory[3];
	valueList[3].actualValue = myBuf[3];

	CheckResults(TestName, valueList, 4);
}

void TestGetDeviceIDFromIndexByteWise()
{
	std::string TestName = "Test Device ID From Index Bytewise";

	heepByte myID0 [STANDARD_ID_SIZE];
	heepByte myID1 [STANDARD_ID_SIZE];
	heepByte myID2 [STANDARD_ID_SIZE];
	heepByte myID3 [STANDARD_ID_SIZE];
	heepByte myID4 [STANDARD_ID_SIZE];

	heepByte destID0 [STANDARD_ID_SIZE];
	heepByte destID1 [STANDARD_ID_SIZE];
	heepByte destID2 [STANDARD_ID_SIZE];
	heepByte destID3 [STANDARD_ID_SIZE];
	heepByte destID4 [STANDARD_ID_SIZE];

	heepByte retID0 [STANDARD_ID_SIZE];
	heepByte retID1 [STANDARD_ID_SIZE];
	heepByte retID2 [STANDARD_ID_SIZE];
	heepByte retID3 [STANDARD_ID_SIZE];
	heepByte retID4 [STANDARD_ID_SIZE];

	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		myID0[i] = i + 1;
		myID1[i] = i + 2;
		myID2[i] = i + 1;
		myID3[i] = i + 3;
		myID4[i] = i + 4;

		destID0[i] = i + 1;
		destID1[i] = i + 2;
		destID2[i] = i + 1;
		destID3[i] = i + 3;
		destID4[i] = i + 4;
	}

	ClearDeviceMemory();

	heepByte retSize0 = GetIndexedDeviceID_Byte(myID0);
	heepByte retSize1 = GetIndexedDeviceID_Byte(myID1);
	heepByte retSize2 = GetIndexedDeviceID_Byte(myID2);
	heepByte retSize3 = GetIndexedDeviceID_Byte(myID3);
	heepByte retSize4 = GetIndexedDeviceID_Byte(myID4);

	GetDeviceIDFromIndex_Byte(myID0, retID0);
	GetDeviceIDFromIndex_Byte(myID1, retID1);
	GetDeviceIDFromIndex_Byte(myID2, retID2);
	GetDeviceIDFromIndex_Byte(myID3, retID3);
	GetDeviceIDFromIndex_Byte(myID4, retID4);

	PrintDeviceMemory();

	ExpectedValue valueList [5];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = CheckBufferEquality(retID0, destID0, STANDARD_ID_SIZE);

	valueList[1].valueName = "Index Value 2";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = CheckBufferEquality(retID1, destID1, STANDARD_ID_SIZE);

	valueList[2].valueName = "Index Value 3";
	valueList[2].expectedValue = 1;
	valueList[2].actualValue = CheckBufferEquality(retID2, destID2, STANDARD_ID_SIZE);

	valueList[3].valueName = "Index Value 4";
	valueList[3].expectedValue = 1;
	valueList[3].actualValue = CheckBufferEquality(retID3, destID3, STANDARD_ID_SIZE);

	valueList[4].valueName = "Index Value 5";
	valueList[4].expectedValue = 1;
	valueList[4].actualValue = CheckBufferEquality(retID4, destID4, STANDARD_ID_SIZE);

	CheckResults(TestName, valueList, 5);
}

void TestAddIndexOrDeviceIDToMemoryBytewise()
{
	std::string TestName = "Test Index or Device to Memory Bytewise";

	heepByte deviceID [STANDARD_ID_SIZE];
	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		deviceID[i] = i + 1;
	}

	ClearDeviceMemory();
	AddIndexOrDeviceIDToMemory_Byte(deviceID);

#ifdef USE_INDEXED_IDS
	int memoryFilled = ID_SIZE + STANDARD_ID_SIZE + 2 + 1;
#else
	int memoryFilled = STANDARD_ID_SIZE;
#endif

	ExpectedValue valueList [1];
	valueList[0].valueName = "Index Value 1";
	valueList[0].expectedValue = memoryFilled;
	valueList[0].actualValue = curFilledMemory;

	CheckResults(TestName, valueList, 1);
}

void TestDeviceNameOpCode_Byte()
{
	std::string TestName = "Device Name Op Code Bytewise";

	ClearDeviceMemory();
	char* deviceName = "Jacob";
	heepByte deviceID[STANDARD_ID_SIZE];
	CreateFakeDeviceID(deviceID);
	SetDeviceNameInMemory_Byte(deviceName, strlen(deviceName), deviceID);

	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [7];
	valueList[0].valueName = "Device Name OpCode";
	valueList[0].expectedValue = DeviceNameOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 5;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Letter One";
	valueList[2].expectedValue = 'J';
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	valueList[3].valueName = "Letter Two";
	valueList[3].expectedValue = 'a';
	valueList[3].actualValue = deviceMemory[memCheckStart + ID_SIZE + 3];

	valueList[4].valueName = "Letter Three";
	valueList[4].expectedValue = 'c';
	valueList[4].actualValue = deviceMemory[memCheckStart + ID_SIZE + 4];

	valueList[5].valueName = "Letter Four";
	valueList[5].expectedValue = 'o';
	valueList[5].actualValue = deviceMemory[memCheckStart + ID_SIZE + 5];

	valueList[6].valueName = "Letter Five";
	valueList[6].expectedValue = 'b';
	valueList[6].actualValue = deviceMemory[memCheckStart + ID_SIZE + 6];

	CheckResults(TestName, valueList, 7);

	PrintDeviceMemory();
}

void TestIconIDOpCode_Byte()
{
	std::string TestName = "Icon ID Op Code Bytewise";

	ClearDeviceMemory();
	heepByte deviceID[STANDARD_ID_SIZE];
	CreateFakeDeviceID(deviceID);
	SetIconIDInMemory_Byte(4, deviceID);

	int memCheckStart = GetMemCounterStart();

	ExpectedValue valueList [3];
	valueList[0].valueName = "Icon ID OpCode";
	valueList[0].expectedValue = IconIDOpCode;
	valueList[0].actualValue = deviceMemory[memCheckStart];

	valueList[1].valueName = "Num Bytes";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[memCheckStart + ID_SIZE + 1];

	valueList[2].valueName = "Letter One";
	valueList[2].expectedValue = 4;
	valueList[2].actualValue = deviceMemory[memCheckStart + ID_SIZE + 2];

	CheckResults(TestName, valueList, 3);
}

void TestDynamicMemory()
{
	TestAddCharToBuffer();
	TestAddCharToDeviceMemory();
	TestClearDeviceMemory();
	TestAddDeviceIDToMemory();
	TestAddDeviceIDToBuffer();
	TestAddIPToDeviceMemory();
	TestDeviceNameOpCode();
	TestIconIDOpCode();
	TestIconDataOpCode();
	TestSetXYOpCode();
	TestSetIPOpCode();
	TestSetVertexOpCode();
	TestGetXYFromMemory();
	TestSkipOpCode();
	TestUpdateXYPosition();
	TestGetVertex();
	TestDefragmentDeviceMemory();
	TestDefragmentDeviceMemoryInMiddle();
	TestDefragmentDeviceMemoryDeviceAtFront();
	TestDefragmentDeviceMemoryAtEnd();
	TestBuildVertexListFromPointers();
	TestControlRegister();
	TestGetIndexFromDeviceID();
	TestGetDeviceIDFromIndex();
	TestByteWiseDeviceEquality();
	TestByteWiseDeviceInequality();
	TestGetDeviceIDFromMemory();
	TestGetDeviceIndexByteWise();
	TestCreateBufferFromNumber();
	TestAddBufferToBuffer();
	TestAddBufferToMemory();
	TestGetDeviceIDFromIndexByteWise();
	TestAddIndexOrDeviceIDToMemoryBytewise();
	TestDeviceNameOpCode_Byte();
	TestIconIDOpCode_Byte();
}