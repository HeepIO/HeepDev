#include "UnitTestSystem.h"
#include "../ActionAndResponseOpCodes.h"

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

void TestGetDeviceIDOctets()
{
	std::string TestName = "Get Device Octets";

	unsigned long ID = 0x01020304;
	unsigned char octet1 = 0;
	unsigned char octet2 = 0;
	unsigned char octet3 = 0;
	unsigned char octet4 = 0;

	GetDeviceIDOctets(ID, octet4, octet3, octet2, octet1);

	ExpectedValue valueList [4];
	valueList[0].valueName = "Octet 4";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = octet4;

	valueList[1].valueName = "Octet 3";
	valueList[1].expectedValue = 2;
	valueList[1].actualValue = octet3;

	valueList[1].valueName = "Octet 2";
	valueList[1].expectedValue = 3;
	valueList[1].actualValue = octet2;

	valueList[1].valueName = "Octet 1";
	valueList[1].expectedValue = 4;
	valueList[1].actualValue = octet1;

	CheckResults(TestName, valueList, 4);
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

	ExpectedValue valueList [11];
	valueList[0].valueName = "Device Name OpCode";
	valueList[0].expectedValue = DeviceNameOpCode;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = deviceMemory[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = deviceMemory[4];

	valueList[5].valueName = "Num Bytes";
	valueList[5].expectedValue = 5;
	valueList[5].actualValue = deviceMemory[5];

	valueList[6].valueName = "Letter One";
	valueList[6].expectedValue = 'J';
	valueList[6].actualValue = deviceMemory[6];

	valueList[7].valueName = "Letter Two";
	valueList[7].expectedValue = 'a';
	valueList[7].actualValue = deviceMemory[7];

	valueList[8].valueName = "Letter Three";
	valueList[8].expectedValue = 'c';
	valueList[8].actualValue = deviceMemory[8];

	valueList[9].valueName = "Letter Four";
	valueList[9].expectedValue = 'o';
	valueList[9].actualValue = deviceMemory[9];

	valueList[10].valueName = "Letter Five";
	valueList[10].expectedValue = 'b';
	valueList[10].actualValue = deviceMemory[10];

	CheckResults(TestName, valueList, 11);
}

void TestIconIDOpCode()
{
	std::string TestName = "Icon ID Op Code";

	ClearDeviceMemory();
	SetIconIDInMemory(4, 0x01020304);

	ExpectedValue valueList [7];
	valueList[0].valueName = "Icon ID OpCode";
	valueList[0].expectedValue = IconIDOpCode;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = deviceMemory[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = deviceMemory[4];

	valueList[5].valueName = "Num Bytes";
	valueList[5].expectedValue = 1;
	valueList[5].actualValue = deviceMemory[5];

	valueList[6].valueName = "Letter One";
	valueList[6].expectedValue = 4;
	valueList[6].actualValue = deviceMemory[6];

	CheckResults(TestName, valueList, 7);
}

void TestIconDataOpCode()
{
	std::string TestName = "Icon Data Op Code";

	ClearDeviceMemory();
	char* deviceName = "Jacob";
	SetIconDataInMemory(deviceName, strlen(deviceName), 0x01020304);

	ExpectedValue valueList [11];
	valueList[0].valueName = "Icon Data OpCode";
	valueList[0].expectedValue = CustomIconDrawingOpCode;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = deviceMemory[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = deviceMemory[4];

	valueList[5].valueName = "Num Bytes";
	valueList[5].expectedValue = 5;
	valueList[5].actualValue = deviceMemory[5];

	valueList[6].valueName = "Letter One";
	valueList[6].expectedValue = 'J';
	valueList[6].actualValue = deviceMemory[6];

	valueList[7].valueName = "Letter Two";
	valueList[7].expectedValue = 'a';
	valueList[7].actualValue = deviceMemory[7];

	valueList[8].valueName = "Letter Three";
	valueList[8].expectedValue = 'c';
	valueList[8].actualValue = deviceMemory[8];

	valueList[9].valueName = "Letter Four";
	valueList[9].expectedValue = 'o';
	valueList[9].actualValue = deviceMemory[9];

	valueList[10].valueName = "Letter Five";
	valueList[10].expectedValue = 'b';
	valueList[10].actualValue = deviceMemory[10];

	CheckResults(TestName, valueList, 11);
}

void TestSetXYOpCode()
{
	std::string TestName = "Front End XY Op Code";

	ClearDeviceMemory();
	SetXYInMemory(312, 513, 0x01020304);

	ExpectedValue valueList [10];
	valueList[0].valueName = "Front End XY OpCode";
	valueList[0].expectedValue = FrontEndPositionOpCode;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = deviceMemory[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = deviceMemory[4];

	valueList[5].valueName = "Num Bytes";
	valueList[5].expectedValue = 4;
	valueList[5].actualValue = deviceMemory[5];

	valueList[6].valueName = "X High Byte";
	valueList[6].expectedValue = 312 >> 8;
	valueList[6].actualValue = deviceMemory[6];

	valueList[7].valueName = "X Low Byte";
	valueList[7].expectedValue = 312%256;
	valueList[7].actualValue = deviceMemory[7];

	valueList[8].valueName = "Y High Byte";
	valueList[8].expectedValue = 513 >> 8;
	valueList[8].actualValue = deviceMemory[8];

	valueList[9].valueName = "Y Low Byte";
	valueList[9].expectedValue = 513%256;
	valueList[9].actualValue = deviceMemory[9];

	CheckResults(TestName, valueList, 10);
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

	ExpectedValue valueList [10];
	valueList[0].valueName = "IP OpCode";
	valueList[0].expectedValue = DeviceIPOpCode;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = deviceMemory[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = deviceMemory[4];

	valueList[5].valueName = "Num Bytes";
	valueList[5].expectedValue = 4;
	valueList[5].actualValue = deviceMemory[5];

	valueList[6].valueName = "Octet 4";
	valueList[6].expectedValue = 192;
	valueList[6].actualValue = deviceMemory[6];

	valueList[7].valueName = "Octet 3";
	valueList[7].expectedValue = 168;
	valueList[7].actualValue = deviceMemory[7];

	valueList[8].valueName = "Octet 2";
	valueList[8].expectedValue = 1;
	valueList[8].actualValue = deviceMemory[8];

	valueList[9].valueName = "Octet 1";
	valueList[9].expectedValue = 100;
	valueList[9].actualValue = deviceMemory[9];

	CheckResults(TestName, valueList, 10);
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

	ExpectedValue valueList [16];
	valueList[0].valueName = "Vertex OpCode";
	valueList[0].expectedValue = VertexOpCode;
	valueList[0].actualValue = deviceMemory[0];

	valueList[1].valueName = "Tx Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = deviceMemory[1];

	valueList[2].valueName = "Tx Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = deviceMemory[2];

	valueList[3].valueName = "Tx Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = deviceMemory[3];

	valueList[4].valueName = "Tx Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = deviceMemory[4];

	valueList[5].valueName = "Num Bytes";
	valueList[5].expectedValue = 10;
	valueList[5].actualValue = deviceMemory[5];

	valueList[6].valueName = "Rx Device ID 1";
	valueList[6].expectedValue = 9;
	valueList[6].actualValue = deviceMemory[6];

	valueList[7].valueName = "Rx Device ID 2";
	valueList[7].expectedValue = 8;
	valueList[7].actualValue = deviceMemory[7];

	valueList[8].valueName = "Rx Device ID 3";
	valueList[8].expectedValue = 7;
	valueList[8].actualValue = deviceMemory[8];

	valueList[9].valueName = "Rx Device ID 4";
	valueList[9].expectedValue = 6;
	valueList[9].actualValue = deviceMemory[9];

	valueList[10].valueName = "Tx Control ID";
	valueList[10].expectedValue = 1;
	valueList[10].actualValue = deviceMemory[10];

	valueList[11].valueName = "Rx Control ID";
	valueList[11].expectedValue = 2;
	valueList[11].actualValue = deviceMemory[11];

	valueList[12].valueName = "IP Octet 4";
	valueList[12].expectedValue = 192;
	valueList[12].actualValue = deviceMemory[12];

	valueList[13].valueName = "IP Octet 3";
	valueList[13].expectedValue = 168;
	valueList[13].actualValue = deviceMemory[13];

	valueList[14].valueName = "IP Octet 2";
	valueList[14].expectedValue = 1;
	valueList[14].actualValue = deviceMemory[14];

	valueList[15].valueName = "IP Octet 1";
	valueList[15].expectedValue = 100;
	valueList[15].actualValue = deviceMemory[15];

	CheckResults(TestName, valueList, 16);
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

	Vertex newVertex;
	int success = GetVertexAtPonter(0, newVertex);

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

	SetVertexInMemory(theVertex);
	ExpectedValue valueList [3];
	unsigned int beforeDeletionMemory = curFilledMemory;
	valueList[0].valueName = "Memory Filled Before Deletion";
	valueList[0].expectedValue = 16;
	valueList[0].actualValue = beforeDeletionMemory;

	DeleteVertexAtPointer(0);
	unsigned int afterDeletionMemory = curFilledMemory;
	valueList[1].valueName = "Memory Filled After Deletion";
	valueList[1].expectedValue = 16;
	valueList[1].actualValue = afterDeletionMemory;

	DefragmentMemory();
	valueList[2].valueName = "Memory Filled after Defragmentation";
	valueList[2].expectedValue = 0;
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
	SetVertexInMemory(theVertex);
	ExpectedValue valueList [3];
	unsigned int beforeDeletionMemory = curFilledMemory;
	valueList[0].valueName = "Memory Filled Before Deletion";
	valueList[0].expectedValue = 39;
	valueList[0].actualValue = beforeDeletionMemory;

	DeleteVertexAtPointer(23);
	unsigned int afterDeletionMemory = curFilledMemory;
	valueList[1].valueName = "Memory Filled After Deletion";
	valueList[1].expectedValue = 39;
	valueList[1].actualValue = afterDeletionMemory;

	DefragmentMemory();
	valueList[2].valueName = "Memory Filled after Defragmentation";
	valueList[2].expectedValue = 23;
	valueList[2].actualValue = curFilledMemory;
	CheckResults(TestName, valueList, 3);
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

	SetVertexInMemory(theVertex);
	SetDeviceNameInMemory("Crowbar", 7, 0x01020304);
	SetIPInMemory(theIP, 0x04030210);
	ExpectedValue valueList [3];
	unsigned int beforeDeletionMemory = curFilledMemory;
	valueList[0].valueName = "Memory Filled Before Deletion";
	valueList[0].expectedValue = 39;
	valueList[0].actualValue = beforeDeletionMemory;

	DeleteVertexAtPointer(0);
	unsigned int afterDeletionMemory = curFilledMemory;
	valueList[1].valueName = "Memory Filled After Deletion";
	valueList[1].expectedValue = 39;
	valueList[1].actualValue = afterDeletionMemory;

	DefragmentMemory();
	valueList[2].valueName = "Memory Filled after Defragmentation";
	valueList[2].expectedValue = 23;
	valueList[2].actualValue = curFilledMemory;

	CheckResults(TestName, valueList, 3);
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
	SetVertexInMemory(theVertex);
	SetIPInMemory(theIP, 0x04030210);
	ExpectedValue valueList [3];
	unsigned int beforeDeletionMemory = curFilledMemory;
	valueList[0].valueName = "Memory Filled Before Deletion";
	valueList[0].expectedValue = 39;
	valueList[0].actualValue = beforeDeletionMemory;

	DeleteVertexAtPointer(13);
	unsigned int afterDeletionMemory = curFilledMemory;
	valueList[1].valueName = "Memory Filled After Deletion";
	valueList[1].expectedValue = 39;
	valueList[1].actualValue = afterDeletionMemory;

	DefragmentMemory();
	valueList[2].valueName = "Memory Filled after Defragmentation";
	valueList[2].expectedValue = 23;
	valueList[2].actualValue = curFilledMemory;
	CheckResults(TestName, valueList, 3);
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

	ExpectedValue valueList [5];
	valueList[0].valueName = "Num Vertices";
	valueList[0].expectedValue = 2;
	valueList[0].actualValue = numberOfVertices;

	valueList[1].valueName = "Vertex 1 Addr";
	valueList[1].expectedValue = 13;
	valueList[1].actualValue = vertexPointerList[0];

	valueList[2].valueName = "Vertex 2 Addr";
	valueList[2].expectedValue = 39;
	valueList[2].actualValue = vertexPointerList[1];

	valueList[3].valueName = "Vertex 1 OpCode Check";
	valueList[3].expectedValue = 0x03;
	valueList[3].actualValue = deviceMemory[vertexPointerList[0]];

	valueList[4].valueName = "Vertex 2 OpCode Check";
	valueList[4].expectedValue = 0x03;
	valueList[4].actualValue = deviceMemory[vertexPointerList[1]];

	CheckResults(TestName, valueList, 5);
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

void TestDynamicMemory()
{
	TestAddCharToBuffer();
	TestAddCharToDeviceMemory();
	TestGetDeviceIDOctets();
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
}