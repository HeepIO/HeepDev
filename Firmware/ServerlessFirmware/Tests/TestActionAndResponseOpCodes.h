#include "../ActionAndResponseOpCodes.h"
#include "../DeviceMemory.h"
#include "../MemoryUtilities.h"
#include "../AutoGeneratedInfo.h"
#include "../DeviceSpecificMemory.h"
#include "../Device.h"
#include "../Heep_API.h"
#include "../Scheduler.h"
#include "UnitTestSystem.h"

void PrintOutputBuffer()
{
	for(int i = 0; i < outputBufferLastByte; i++)
	{
		cout << (int)outputBuffer[i] << " ";
	}
	cout << endl;
}

void PrintBuffer(unsigned char *buffer, unsigned long bufferSize)
{
	for(int i = 0; i < bufferSize; i++)
	{
		cout << (int)buffer[i] << " ";
	}
	cout << endl;
}

void TestClearOutputBufferAndAddChar()
{
	std::string TestName = "Add Char to Output Buffer and Clear";

	ClearOutputBuffer();
	AddNewCharToOutputBuffer('3');

	ExpectedValue valueList [2];
	valueList[0].valueName = "Num Chars before clear";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = outputBufferLastByte;

	ClearOutputBuffer();

	valueList[1].valueName = "Num Chars after clear";
	valueList[1].expectedValue = 0;
	valueList[1].actualValue = outputBufferLastByte;

	CheckResults(TestName, valueList, 2);
}

void TestMemoryDumpROP()
{
	std::string TestName = "Memory Dump ROP";

	for(int i = 0; i < STANDARD_ID_SIZE; i++)
	{
		deviceIDByte[i] = i+1;
	}

	ClearDeviceMemory();
	SetDeviceName("Jacob");
	ClearOutputBuffer();
	FillOutputBufferWithMemoryDump();

	PrintOutputBuffer();

	ExpectedValue valueList[10];
	valueList[0].valueName = "Memory Dump";
	valueList[0].expectedValue = MemoryDumpOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = outputBuffer[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = outputBuffer[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = outputBuffer[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = outputBuffer[4];

	CheckResults(TestName, valueList, 5);
}

void TestHeepDeviceCOP()
{
	std::string TestName = "Is Heep Device COP";

	ClearDeviceMemory();
	SetDeviceName("Jacob");
	ClearOutputBuffer();
	ClearInputBuffer();

	inputBuffer[0] = 0x09;
	inputBuffer[1] = 0x00;
	ExecuteControlOpCodes();

	ExpectedValue valueList[10];
	valueList[0].valueName = "Memory Dump";
	valueList[0].expectedValue = MemoryDumpOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "Device ID 1";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = outputBuffer[1];

	valueList[2].valueName = "Device ID 2";
	valueList[2].expectedValue = 2;
	valueList[2].actualValue = outputBuffer[2];

	valueList[3].valueName = "Device ID 3";
	valueList[3].expectedValue = 3;
	valueList[3].actualValue = outputBuffer[3];

	valueList[4].valueName = "Device ID 4";
	valueList[4].expectedValue = 4;
	valueList[4].actualValue = outputBuffer[4];

	CheckResults(TestName, valueList, 5);
}

void TestNumberFromBuffer()
{
	std::string TestName = "Get Number from Buffer";

	unsigned char myBuffer [4];
	myBuffer[0] = 0x01;
	myBuffer[1] = 0xFF;

	unsigned int counter = 0;
	unsigned int retVal = GetNumberFromBuffer(myBuffer, &counter, 2);

	ExpectedValue valueList[1];
	valueList[0].valueName = "Returned Number";
	valueList[0].expectedValue = 511;
	valueList[0].actualValue = retVal;

	CheckResults(TestName, valueList, 1);
}

void TestSetValSuccess()
{
	std::string TestName = "Test Set Val COP";

	ClearControls();
	SetDeviceName("Test");
	Control theControl;
	theControl.controlName = "Test Control";
	theControl.controlID = 0;
	theControl.controlDirection = 1;
	theControl.controlType = 1;
	theControl.highValue= 100;
	theControl.lowValue = 0;
	theControl.curValue = 50;
	AddControl(theControl);

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;// OpCode
	inputBuffer[1] = 0x05;
	inputBuffer[2] = 0x00; // Destination ID
	inputBuffer[3] = 0x00; // Source Control Type
	inputBuffer[4] = 0x04; // Source High Val
	inputBuffer[5] = 0x00; // Source Low Val
	inputBuffer[6] = 0x03; // Value
	inputBuffer[7] = 0x05; // Random junk for edge case test
	ExecuteControlOpCodes();

	ExpectedValue valueList[2];
	valueList[0].valueName = "Returned Op Code";
	valueList[0].expectedValue = SuccessOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "Control Value";
	valueList[1].expectedValue = 75;
	valueList[1].actualValue = GetControlValueByID(0);

	CheckResults(TestName, valueList, 2);
}

void TestSetValFailure()
{
	std::string TestName = "Test Set Val COP Failures";

	ClearControls();
	SetDeviceName("Test");
	Control theControl;
	theControl.controlName = "Test Control";
	theControl.controlID = 0;
	theControl.controlDirection = 1;
	theControl.controlType = 1;
	theControl.highValue= 100;
	theControl.lowValue = 0;
	theControl.curValue = 50;
	AddControl(theControl);

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;
	inputBuffer[1] = 0x02;
	inputBuffer[2] = 0x01;
	inputBuffer[3] = 0x00; // Source Control Type
	inputBuffer[4] = 0x04; // Source High Val
	inputBuffer[5] = 0x00; // Source Low Val
	inputBuffer[6] = 0x03; // Value
	ExecuteControlOpCodes();

	ExpectedValue valueList[2];
	valueList[0].valueName = "Returned Op Code";
	valueList[0].expectedValue = ErrorOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "Control Value";
	valueList[1].expectedValue = 50;
	valueList[1].actualValue = GetControlValueByID(0);

	CheckResults(TestName, valueList, 2);
}

void TestSetPositionOpCode()
{
	std::string TestName = "Test Set Position COP";

	ClearControls();
	SetDeviceName("Test");

	ClearInputBuffer();
	inputBuffer[0] = 0x0B;
	inputBuffer[1] = 0x04;
	inputBuffer[2] = 0x01;
	inputBuffer[3] = 0x01;
	inputBuffer[4] = 0x10;
	inputBuffer[5] = 0x10;
	ExecuteControlOpCodes();
	heepByte deviceID [STANDARD_ID_SIZE] = {0x06, 0x04, 0x06, 0x01};
	int x = 0; int y = 0; unsigned int xyMemPosition = 0; 
	GetXYFromMemory_Byte(&x, &y, deviceID, &xyMemPosition);

	ExpectedValue valueList[4];
	valueList[0].valueName = "x";
	valueList[0].expectedValue = 0x0101;
	valueList[0].actualValue = x;

	valueList[1].valueName = "y";
	valueList[1].expectedValue = 0x1010;
	valueList[1].actualValue = y;

	ClearInputBuffer();
	inputBuffer[0] = 0x0B;
	inputBuffer[1] = 0x04;
	inputBuffer[2] = 0xF1;
	inputBuffer[3] = 0x02;
	inputBuffer[4] = 0xB2;
	inputBuffer[5] = 0x3C;
	ExecuteControlOpCodes();
	GetXYFromMemory_Byte(&x, &y, deviceID, &xyMemPosition);

	valueList[2].valueName = "x";
	valueList[2].expectedValue = 0xF102;
	valueList[2].actualValue = x;

	valueList[3].valueName = "y";
	valueList[3].expectedValue = 0xB23C;
	valueList[3].actualValue = y;

	CheckResults(TestName, valueList, 4);
}

void TestSetVertxCOP()
{
	std::string TestName = "Test Set Vertex COP";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();

	inputBuffer[0] = 0x0C;
	inputBuffer[1] = 0x04;

	inputBuffer[2] = 0xF1;
	inputBuffer[3] = 0x02;
	inputBuffer[4] = 0xB2;
	inputBuffer[5] = 0x3C;

	inputBuffer[6] = 0x1A;
	inputBuffer[7] = 0x2D;
	inputBuffer[8] = 0x40;
	inputBuffer[9] = 0x02;

	inputBuffer[10] = 0x01;
	inputBuffer[11] = 0x02;

	inputBuffer[12] = 0xC0;
	inputBuffer[13] = 0xD0;
	inputBuffer[14] = 0x20;
	inputBuffer[15] = 0x02;
	ExecuteControlOpCodes();

	Vertex_Byte newVertex;
	int success = GetVertexAtPointer_Byte(vertexPointerList[0], &newVertex);

	heepByte trueTxID [STANDARD_ID_SIZE] = {0xF1,0x02,0xB2,0x3C};
	heepByte trueRxID [STANDARD_ID_SIZE] = {0x1A, 0x2D, 0x40, 0x02};

	ExpectedValue valueList [8];
	valueList[0].valueName = "TXID";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = CheckBufferEquality(newVertex.txID, trueTxID, STANDARD_ID_SIZE);

	valueList[1].valueName = "RXID";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = CheckBufferEquality(newVertex.rxID, trueRxID, STANDARD_ID_SIZE);

	valueList[2].valueName = "TX Control ID";
	valueList[2].expectedValue = 0x01;
	valueList[2].actualValue = newVertex.txControlID;

	valueList[3].valueName = "RX Control ID";
	valueList[3].expectedValue = 0x02;
	valueList[3].actualValue = newVertex.rxControlID;

	valueList[4].valueName = "IP 4";
	valueList[4].expectedValue = 0xC0;
	valueList[4].actualValue = newVertex.rxIPAddress.Octet4;

	valueList[5].valueName = "IP 3";
	valueList[5].expectedValue = 0xD0;
	valueList[5].actualValue = newVertex.rxIPAddress.Octet3;

	valueList[6].valueName = "IP 2";
	valueList[6].expectedValue = 0x20;
	valueList[6].actualValue = newVertex.rxIPAddress.Octet2;

	valueList[7].valueName = "IP 1";
	valueList[7].expectedValue = 0x02;
	valueList[7].actualValue = newVertex.rxIPAddress.Octet1;

	CheckResults(TestName, valueList, 8);
}

void TestAddMOPOpCode()
{
	std::string TestName = "Test Set MOP COP";

	ClearDeviceMemory();
	ClearInputBuffer();

	// Add a random clients name
	inputBuffer[0] = 0x13;
	inputBuffer[1] = 0x0B;

	inputBuffer[2] = 0x06;
	inputBuffer[3] = 0x01;
	inputBuffer[4] = 0x02;
	inputBuffer[5] = 0x03;
	inputBuffer[6] = 0x04;
	inputBuffer[7] = 0x05;

	inputBuffer[8] = 'J';
	inputBuffer[9] = 'a';
	inputBuffer[10] = 'm';

	inputBuffer[11] = 'e';
	inputBuffer[12] = 's';

	unsigned int beforeMemory = curFilledMemory;

	ExecuteControlOpCodes();

	unsigned int afterMemory = curFilledMemory;

	// Traverse the new memory by updating XY twice
	heepByte deviceID[STANDARD_ID_SIZE] = {0x01, 0x02, 0x03, 0x04};
	UpdateXYInMemory_Byte(1234, 161, deviceID);
	unsigned int beforeTraversal = curFilledMemory;

	UpdateXYInMemory_Byte(2321, 5101, deviceID);
	unsigned int afterTraveresal = curFilledMemory;

#ifdef USE_INDEXED_IDS
	unsigned int expectedMemory = ID_SIZE + STANDARD_ID_SIZE + 2 + 2 + ID_SIZE + 5 + 2 + ID_SIZE + 4;
#else
	unsigned int expectedMemory = 21;
#endif

	ExpectedValue valueList [4];
	valueList[0].valueName = "Before Memory Size";
	valueList[0].expectedValue = 0;
	valueList[0].actualValue = beforeMemory;

	valueList[1].valueName = "After Memory Size";
#ifdef USE_INDEXED_IDS
	valueList[1].expectedValue = ID_SIZE + STANDARD_ID_SIZE + 2 + 2 + ID_SIZE + 5;
#else
	valueList[1].expectedValue = 11;
#endif
	valueList[1].actualValue = afterMemory;

	valueList[2].valueName = "Before Traversal Memory Size";
	valueList[2].expectedValue = expectedMemory;
	valueList[2].actualValue = beforeTraversal;

	valueList[3].valueName = "After Traversal Memory Size";
	valueList[3].expectedValue = expectedMemory;
	valueList[3].actualValue = afterTraveresal;

	CheckResults(TestName, valueList, 4);
}

void TestDeleteMOPOpCode()
{
	std::string TestName = "Test Delete MOP COP";

	ClearDeviceMemory();
	ClearInputBuffer();

	heepByte deviceID[STANDARD_ID_SIZE] = {0x01, 0x02, 0x03, 0x04};
	char* device1Name = "Jacob";
	SetDeviceNameInMemory_Byte(device1Name, strlen(device1Name), deviceID);
	char* device2Name = "James";
	SetDeviceNameInMemory_Byte(device2Name, strlen(device2Name), deviceID);
	UpdateXYInMemory_Byte(1234, 161, deviceID);

	// Add a random clients name
	inputBuffer[0] = 0x15;
	inputBuffer[1] = 0x0B;

	inputBuffer[2] = 0x06;
	inputBuffer[3] = 0x01;
	inputBuffer[4] = 0x02;
	inputBuffer[5] = 0x03;
	inputBuffer[6] = 0x04;
	inputBuffer[7] = 0x05;

	inputBuffer[8] = 'J';
	inputBuffer[9] = 'a';
	inputBuffer[10] = 'm';

	inputBuffer[11] = 'e';
	inputBuffer[12] = 's';

#ifdef USE_INDEXED_IDS
	unsigned char valAtSpotBeforeDeleteion = deviceMemory[15];
#else
	unsigned char valAtSpotBeforeDeleteion = deviceMemory[11];
#endif
	
	ExecuteControlOpCodes();

#ifdef USE_INDEXED_IDS
	unsigned char valAtSpotAfterDeletion = deviceMemory[15];
#else
	unsigned char valAtSpotAfterDeletion = deviceMemory[11];
#endif

	ExpectedValue valueList [2];
	valueList[0].valueName = "Before OpCode";
	valueList[0].expectedValue = DeviceNameOpCode;
	valueList[0].actualValue = valAtSpotBeforeDeleteion;

	valueList[1].valueName = "After OpCode";
	valueList[1].expectedValue = FragmentOpCode;
	valueList[1].actualValue = valAtSpotAfterDeletion;

	CheckResults(TestName, valueList, 2);
}

void TestGetAnalyticsString()
{
#ifdef USE_ANALYTICS
	std::string TestName = "Test Analytics Get String and Delete";

	ClearDeviceMemory();

	simMillis = 0x01020304050607;
	heepByte deviceID1[STANDARD_ID_SIZE];
	CreateFakeDeviceID(deviceID1);
	SetAnalyticsDataControlValueInMemory_Byte(0, 4, deviceID1);
	char* deviceName = "Jacob";
	SetDeviceNameInMemory_Byte(deviceName, strlen(deviceName), deviceID1);
	SetAnalyticsDataControlValueInMemory_Byte(1, 2, deviceID1);

	int counter = GetMemCounterStart();
	AddAnalyticsStringToOutputBufferAndDeleteMOPs();
	DefragmentMemory();

	ExpectedValue valueList [3];
	valueList[0].valueName = "Output Buffer Size";
	valueList[0].expectedValue = 36;
	valueList[0].actualValue = outputBufferLastByte;

	valueList[1].valueName = "Cur Filled Memory Defragmented";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = curFilledMemory < 20; // Memory reduced from when analytics were being captured

	AddAnalyticsStringToOutputBufferAndDeleteMOPs();

	valueList[2].valueName = "Analytics String After Delete";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = outputBufferLastByte;

	CheckResults(TestName, valueList, 3);
#endif
}

void TestAddWiFiCOP()
{
	std::string TestName = "Test Add WiFi COP";

	ClearDeviceMemory();

	ClearInputBuffer();
	inputBuffer[0] = SetWiFiDataOpCode;
	inputBuffer[1] = 17;
	inputBuffer[2] = 0;
	inputBuffer[3] = 8;
	inputBuffer[4] = (unsigned char)'M';
	inputBuffer[5] = (unsigned char)'a';
	inputBuffer[6] = (unsigned char)'g';
	inputBuffer[7] = (unsigned char)'D';
	inputBuffer[8] = (unsigned char)'y';
	inputBuffer[9] = (unsigned char)'l';
	inputBuffer[10] = (unsigned char)'a';
	inputBuffer[11] = (unsigned char)'n';
	inputBuffer[12] = 6;
	inputBuffer[13] = (unsigned char)'S';
	inputBuffer[14] = (unsigned char)'e';
	inputBuffer[15] = (unsigned char)'c';
	inputBuffer[16] = (unsigned char)'r';
	inputBuffer[17] = (unsigned char)'e';
	inputBuffer[18] = (unsigned char)'t';

	ExecuteControlOpCodes();

	char RetrievedSSID [20];
	char RetrievedPassword [20];
	for(int i = 0; i < 20; i++)
		RetrievedSSID[i] = RetrievedPassword[i] = '\0';

	int didFindMemory = GetWiFiFromMemory(RetrievedSSID, RetrievedPassword, 0);
	std::string RetreivedSSIDStr(RetrievedSSID);
	std::string RetreivedPasswordStr(RetrievedPassword);

	std::string expectedSSID = "MagDylan";
	ExpectedValue valueList [3];
	valueList[0].valueName = "SSID Match";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = RetreivedSSIDStr == expectedSSID;

	valueList[1].valueName = "Password Match";
	valueList[1].expectedValue = 1;
	valueList[1].actualValue = RetreivedPasswordStr == std::string("Secret"); 

	valueList[2].valueName = "Found Match";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = didFindMemory;

	CheckResults(TestName, valueList, 3);
}

void TestDeviceNameCOP()
{
	std::string TestName = "Test Add Device Name COP";

	ClearDeviceMemory();

	ClearInputBuffer();
	inputBuffer[0] = SetNameOpCode;
	inputBuffer[1] = 5;
	inputBuffer[2] = (unsigned char)'J';
	inputBuffer[3] = (unsigned char)'a';
	inputBuffer[4] = (unsigned char)'c';
	inputBuffer[6] = (unsigned char)'o';
	inputBuffer[7] = (unsigned char)'b';

	ExecuteControlOpCodes();
	

#ifdef USE_INDEXED_IDS
	unsigned char FirstLetterOfName = deviceMemory[10];
#else
	unsigned char FirstLetterOfName = deviceMemory[6];
#endif

	ExpectedValue valueList [1];
	valueList[0].valueName = "First Letter";
	valueList[0].expectedValue = 'J';
	valueList[0].actualValue = FirstLetterOfName;

	CheckResults(TestName, valueList, 1);
}

// Note: If this test fails, it may be due to the vertex pointer list being too small
// 	     Enough vertex pointers must be supported to fill memory
void TestSetVertexOverflow()
{
	std::string TestName = "Test Vertex Overflow Detection";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();

	inputBuffer[0] = 0x0C;
	inputBuffer[1] = 0x04;

	inputBuffer[2] = 0xF1;
	inputBuffer[3] = 0x02;
	inputBuffer[4] = 0xB2;
	inputBuffer[5] = 0x3C;

	inputBuffer[6] = 0x1A;
	inputBuffer[7] = 0x2D;
	inputBuffer[8] = 0x40;
	inputBuffer[9] = 0x02;

	inputBuffer[10] = 0x01;
	inputBuffer[11] = 0x02;

	inputBuffer[12] = 0xC0;
	inputBuffer[13] = 0xD0;
	inputBuffer[14] = 0x20;
	inputBuffer[15] = 0x02;

	heepByte firstROP = outputBuffer[0];
	for(int i = 0; i < 20000; i++)
	{
		ExecuteControlOpCodes();
	}
	heepByte lastROP = outputBuffer[0];

	ExpectedValue valueList [3];
	valueList[0].valueName = "Less than max memory";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = curFilledMemory <= MAX_MEMORY;

	valueList[1].valueName = "ROP Should be Failure";
	valueList[1].expectedValue = ErrorOpCode;
	valueList[1].actualValue = lastROP;

	valueList[2].valueName = "ROP Should be Success";
	valueList[2].expectedValue = SuccessOpCode;
	valueList[2].actualValue = firstROP;

	CheckResults(TestName, valueList, 3);
}

// Test Currently fails on indexed code. Need to handle overflow from adding a new
// Compressed device ID
void CheckSetPositionOverflowHandling()
{
	std::string TestName = "Test Position Overflow Detection";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();

	inputBuffer[0] = SetPositionOpCode;
	inputBuffer[1] = 0x04;

	inputBuffer[2] = 0xA0;
	inputBuffer[3] = 0x02;
	inputBuffer[4] = 0xB2;
	inputBuffer[5] = 0x3C;

	ExecuteControlOpCodes();

	heepByte shouldBeSuccessCode = outputBuffer[0];

	ClearDeviceMemory();

	SetDeviceNameInMemory_Byte("FOUR", 4, deviceIDByte);

	for(int i = curFilledMemory; i < MAX_MEMORY-3; i++)
	{
		AddNewCharToMemory('H');
	}

	inputBuffer[0] = SetPositionOpCode;
	inputBuffer[1] = 0x04;

	inputBuffer[2] = 0xA0;
	inputBuffer[3] = 0x02;
	inputBuffer[4] = 0xB2;
	inputBuffer[5] = 0x3C;

	ExecuteControlOpCodes();

	heepByte shouldbeFailureCode = outputBuffer[0];

	ExpectedValue valueList [3];
	valueList[0].valueName = "Less than max memory";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = curFilledMemory <= MAX_MEMORY;

	valueList[1].valueName = "ROP Should be Failure";
	valueList[1].expectedValue = ErrorOpCode;
	valueList[1].actualValue = shouldbeFailureCode;

	valueList[2].valueName = "ROP Should be Success";
	valueList[2].expectedValue = SuccessOpCode;
	valueList[2].actualValue = shouldBeSuccessCode;

	CheckResults(TestName, valueList, 3);
}

void TestWiFiOverflowDetection()
{
	std::string TestName = "Test WiFi Overflow Detection";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();

	inputBuffer[0] = SetWiFiDataOpCode;
	inputBuffer[1] = 17;
	inputBuffer[2] = 0;
	inputBuffer[3] = 8;
	inputBuffer[4] = (unsigned char)'M';
	inputBuffer[5] = (unsigned char)'a';
	inputBuffer[6] = (unsigned char)'g';
	inputBuffer[7] = (unsigned char)'D';
	inputBuffer[8] = (unsigned char)'y';
	inputBuffer[9] = (unsigned char)'l';
	inputBuffer[10] = (unsigned char)'a';
	inputBuffer[11] = (unsigned char)'n';
	inputBuffer[12] = 6;
	inputBuffer[13] = (unsigned char)'S';
	inputBuffer[14] = (unsigned char)'e';
	inputBuffer[15] = (unsigned char)'c';
	inputBuffer[16] = (unsigned char)'r';
	inputBuffer[17] = (unsigned char)'e';
	inputBuffer[18] = (unsigned char)'t';

	ExecuteControlOpCodes();

	heepByte shouldBeSuccessCode = outputBuffer[0];

	ClearDeviceMemory();

	inputBuffer[0] = SetWiFiDataOpCode;
	inputBuffer[1] = 17;
	inputBuffer[2] = 0;
	inputBuffer[3] = 8;
	inputBuffer[4] = (unsigned char)'M';
	inputBuffer[5] = (unsigned char)'a';
	inputBuffer[6] = (unsigned char)'g';
	inputBuffer[7] = (unsigned char)'D';
	inputBuffer[8] = (unsigned char)'y';
	inputBuffer[9] = (unsigned char)'l';
	inputBuffer[10] = (unsigned char)'a';
	inputBuffer[11] = (unsigned char)'n';
	inputBuffer[12] = 6;
	inputBuffer[13] = (unsigned char)'S';
	inputBuffer[14] = (unsigned char)'e';
	inputBuffer[15] = (unsigned char)'c';
	inputBuffer[16] = (unsigned char)'r';
	inputBuffer[17] = (unsigned char)'e';
	inputBuffer[18] = (unsigned char)'t';

	for(int i =0; i<20000;i++)
		ExecuteControlOpCodes();

	heepByte shouldbeFailureCode = outputBuffer[0];

	ExpectedValue valueList [3];
	valueList[0].valueName = "Less than max memory";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = curFilledMemory <= MAX_MEMORY;

	valueList[1].valueName = "ROP Should be Failure";
	valueList[1].expectedValue = ErrorOpCode;
	valueList[1].actualValue = shouldbeFailureCode;

	valueList[2].valueName = "ROP Should be Success";
	valueList[2].expectedValue = SuccessOpCode;
	valueList[2].actualValue = shouldBeSuccessCode;

	CheckResults(TestName, valueList, 3);
}

void TestNameOverflowDetection()
{
	std::string TestName = "Test Name Overflow Detection";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();

	inputBuffer[0] = SetNameOpCode;
	inputBuffer[1] = 5;
	inputBuffer[2] = (unsigned char)'J';
	inputBuffer[3] = (unsigned char)'a';
	inputBuffer[4] = (unsigned char)'c';
	inputBuffer[6] = (unsigned char)'o';
	inputBuffer[7] = (unsigned char)'b';

	ExecuteControlOpCodes();

	heepByte shouldBeSuccessCode = outputBuffer[0];

	ClearDeviceMemory();
	SetXYInMemory_Byte(20, 30, deviceIDByte);

	for(int i = curFilledMemory; i < MAX_MEMORY-3; i++)
	{
		AddNewCharToMemory('H');
	}

	inputBuffer[0] = SetNameOpCode;
	inputBuffer[1] = 5;
	inputBuffer[2] = (unsigned char)'J';
	inputBuffer[3] = (unsigned char)'a';
	inputBuffer[4] = (unsigned char)'c';
	inputBuffer[6] = (unsigned char)'o';
	inputBuffer[7] = (unsigned char)'b';

	ExecuteControlOpCodes();

	heepByte shouldbeFailureCode = outputBuffer[0];

	ExpectedValue valueList [3];
	valueList[0].valueName = "Less than max memory";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = curFilledMemory <= MAX_MEMORY;

	valueList[1].valueName = "ROP Should be Failure";
	valueList[1].expectedValue = ErrorOpCode;
	valueList[1].actualValue = shouldbeFailureCode;

	valueList[2].valueName = "ROP Should be Success";
	valueList[2].expectedValue = SuccessOpCode;
	valueList[2].actualValue = shouldBeSuccessCode;

	CheckResults(TestName, valueList, 3);
}

void TestGetScaledValue()
{
	std::string TestName = "Test Scale Input Values";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();
	ClearControls();
	
	SetDeviceName("Test");
	Control theControl;
	theControl.controlName = "Test Control";
	theControl.controlID = 0;
	theControl.controlDirection = 1;
	theControl.controlType = 1;
	theControl.highValue= 1;
	theControl.lowValue = 0;
	theControl.curValue = 50;
	AddControl(theControl);

	Control theControl2;
	theControl2.controlName = "Test Control 2";
	theControl2.controlID = 1;
	theControl2.controlDirection = 1;
	theControl2.controlType = 1;
	theControl2.highValue= 100;
	theControl2.lowValue = 0;
	theControl2.curValue = 50;
	AddControl(theControl2);

	int GetScaledValue(unsigned char controlID, unsigned int value, unsigned int highValue, unsigned int lowValue);

	int shouldBe1 = GetScaledValue(0, 25, 50, 0);
	int shouldBe50 = GetScaledValue(1, 25, 50, 0);
	int shouldBe0 = GetScaledValue(0, 24, 50, 0);
	int shouldBe1Two = GetScaledValue(0, 1, 2, 0);
	int shouldBe100 = GetScaledValue(1, 3, 2, 0);

	ExpectedValue valueList [5];
	valueList[0].valueName = "Scaled 1";
	valueList[0].expectedValue = 1;
	valueList[0].actualValue = shouldBe1;

	valueList[1].valueName = "Scaled 2";
	valueList[1].expectedValue = 50;
	valueList[1].actualValue = shouldBe50;

	valueList[2].valueName = "Scaled 3";
	valueList[2].expectedValue = 0;
	valueList[2].actualValue = shouldBe0;

	valueList[3].valueName = "Scaled 4";
	valueList[3].expectedValue = 1;
	valueList[3].actualValue = shouldBe1Two;

	valueList[4].valueName = "Scaled 5";
	valueList[4].expectedValue = 100;
	valueList[4].actualValue = shouldBe100;

	CheckResults(TestName, valueList, 4);
}

void TestSetValBuffer()
{
	std::string TestName = "Test Set Val Buffer";

	ClearVertices();
	ClearDeviceMemory();
	ClearInputBuffer();
	ClearControls();
	
	heepByte bufferControl [10];

	SetDeviceName("Test");
	Control theControl;
	theControl.controlName = "Test Control";
	theControl.controlID = 0;
	theControl.controlDirection = 1;
	theControl.controlType = 2;
	theControl.highValue= 10;
	theControl.controlBuffer = bufferControl;
	AddControl(theControl);

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;// OpCode
	inputBuffer[1] = 0x05;
	inputBuffer[2] = 0x00; // Destination ID
	inputBuffer[3] = 0x00; // Source Control Type (Not buffer, should fail)
	inputBuffer[4] = 0x04; // Source High Val
	inputBuffer[5] = 0x00; // Source Low Val
	inputBuffer[6] = 0x03; // Value
	inputBuffer[7] = 0x05; // Random junk for edge case test
	ExecuteControlOpCodes();

	int shoudlBeFailureROP = outputBuffer[0];

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;// OpCode
	inputBuffer[1] = 12;
	inputBuffer[2] = 0x00; // Destination ID
	inputBuffer[3] = 0x02; // Source Control Type
	for(int i = 0; i < 10; i++)
	{
		inputBuffer[i+4] = i;
	}
	ExecuteControlOpCodes();

	int shouldBeSuccessROP = outputBuffer[0];

	ExpectedValue valueList[12];
	valueList[0].valueName = "Returned Op Code";
	valueList[0].expectedValue = ErrorOpCode;
	valueList[0].actualValue = shoudlBeFailureROP;

	valueList[1].valueName = "Control Value";
	valueList[1].expectedValue = SuccessOpCode;
	valueList[1].actualValue = shouldBeSuccessROP;

	for(int i = 0; i < 10; i++)
	{
		valueList[i+2].valueName = std::string("Value ") + std::to_string(i);
		valueList[i+2].expectedValue = i;
		valueList[i+2].actualValue = controlList[0].controlBuffer[i];
	}

	CheckResults(TestName, valueList, 12);

}

void TestActionAndResponseOpCodes()
{
	TestClearOutputBufferAndAddChar();
	TestMemoryDumpROP();
	TestHeepDeviceCOP();
	TestNumberFromBuffer();
	TestSetValSuccess();
	TestSetValFailure();
	TestSetPositionOpCode();
	TestSetVertxCOP();
	TestAddMOPOpCode();
	TestDeleteMOPOpCode();
	TestGetAnalyticsString();
	TestAddWiFiCOP();
	TestDeviceNameCOP();
	TestSetVertexOverflow();
	CheckSetPositionOverflowHandling();
	TestWiFiOverflowDetection();
	TestNameOverflowDetection();
	TestGetScaledValue();
	TestSetValBuffer();
}