#include "../ActionAndResponseOpCodes.h"
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

int AddAccessCodeToBuffer(heepByte* buffer, int startVal)
{
	for(int i = 0; i < ACCESS_CODE_SIZE; i++)
	{
		buffer[i+startVal] = i;
	}

	return startVal+ACCESS_CODE_SIZE;
}

void GenerateAccessCode()
{
	for(int i = 0; i < ACCESS_CODE_SIZE; i++)
	{
		accessCode[i] = i;
	}
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

	adminAccessCode[0] = 1;
	adminAccessCode[1] = 2;

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

	GenerateAccessCode();

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0x00;
	inputBuffer[newCount++] = 0x04;
	ExecuteControlOpCodes();

	ExpectedValue valueList[2];
	valueList[0].valueName = "Returned Op Code";
	valueList[0].expectedValue = SuccessOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "Control Value";
	valueList[1].expectedValue = 4;
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

	GenerateAccessCode();

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0x01;
	inputBuffer[newCount++] = 0x04;
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

	GenerateAccessCode();

	ClearInputBuffer();
	inputBuffer[0] = 0x0B;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x04;
	inputBuffer[newCount++] = 0x01;
	inputBuffer[newCount++] = 0x01;
	inputBuffer[newCount++] = 0x10;
	inputBuffer[newCount++] = 0x10;
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
	newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x04;
	inputBuffer[newCount++] = 0xF1;
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0xB2;
	inputBuffer[newCount++] = 0x3C;
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

	GenerateAccessCode();

	inputBuffer[0] = 0x0C;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x04;

	inputBuffer[newCount++] = 0xF1;
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0xB2;
	inputBuffer[newCount++] = 0x3C;

	inputBuffer[newCount++] = 0x1A;
	inputBuffer[newCount++] = 0x2D;
	inputBuffer[newCount++] = 0x40;
	inputBuffer[newCount++] = 0x02;

	inputBuffer[newCount++] = 0x01;
	inputBuffer[newCount++] = 0x02;

	inputBuffer[newCount++] = 0xC0;
	inputBuffer[newCount++] = 0xD0;
	inputBuffer[newCount++] = 0x20;
	inputBuffer[newCount++] = 0x02;
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

	GenerateAccessCode();

	// Add a random clients name
	inputBuffer[0] = 0x13;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x0B;

	inputBuffer[newCount++] = 0x06;
	inputBuffer[newCount++] = 0x01;
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0x03;
	inputBuffer[newCount++] = 0x04;
	inputBuffer[newCount++] = 0x05;

	inputBuffer[newCount++] = 'J';
	inputBuffer[newCount++] = 'a';
	inputBuffer[newCount++] = 'm';

	inputBuffer[newCount++] = 'e';
	inputBuffer[newCount++] = 's';

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

	GenerateAccessCode();

	heepByte deviceID[STANDARD_ID_SIZE] = {0x01, 0x02, 0x03, 0x04};
	char* device1Name = "Jacob";
	SetDeviceNameInMemory_Byte(device1Name, strlen(device1Name), deviceID);
	char* device2Name = "James";
	SetDeviceNameInMemory_Byte(device2Name, strlen(device2Name), deviceID);
	UpdateXYInMemory_Byte(1234, 161, deviceID);

	// Add a random clients name
	inputBuffer[0] = 0x15;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = 0x0B;

	inputBuffer[newCount++] = 0x06;
	inputBuffer[newCount++] = 0x01;
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0x03;
	inputBuffer[newCount++] = 0x04;
	inputBuffer[newCount++] = 0x05;

	inputBuffer[newCount++] = 'J';
	inputBuffer[newCount++] = 'a';
	inputBuffer[newCount++] = 'm';

	inputBuffer[newCount++] = 'e';
	inputBuffer[newCount++] = 's';

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

void TestMasterAccessCodeVerification()
{
	std::string TestName = "Test Master Access Code Verification";

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

	GET_MASTER_ACCESS_CODE

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;
	unsigned int newCount = 1;
	unsigned int txCount = 0;
	AddBufferToBuffer(inputBuffer, masterAccessCode, ACCESS_CODE_SIZE, &newCount, &txCount);
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0x00;
	inputBuffer[newCount++] = 0x04;
	ExecuteControlOpCodes();

	ExpectedValue valueList[2];
	valueList[0].valueName = "Returned Op Code";
	valueList[0].expectedValue = SuccessOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "Control Value";
	valueList[1].expectedValue = 4;
	valueList[1].actualValue = GetControlValueByID(0);

	CheckResults(TestName, valueList, 2);
}

void TestAccessCodeVerificationFailure()
{
	std::string TestName = "Test Access Code Verification Failure";

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

	GET_MASTER_ACCESS_CODE
	masterAccessCode[2] = 0;

	ClearInputBuffer();
	inputBuffer[0] = 0x0A;
	unsigned int newCount = 1;
	unsigned int txCount = 0;
	AddBufferToBuffer(inputBuffer, masterAccessCode, ACCESS_CODE_SIZE, &newCount, &txCount);
	inputBuffer[newCount++] = 0x02;
	inputBuffer[newCount++] = 0x00;
	inputBuffer[newCount++] = 0x04;
	ExecuteControlOpCodes();

	ExpectedValue valueList[1];
	valueList[0].valueName = "Returned Op Code";
	valueList[0].expectedValue = ErrorOpCode;
	valueList[0].actualValue = outputBuffer[0];

	CheckResults(TestName, valueList, 1);
}

void TestSetAdminIDOpCode()
{
	std::string TestName = "Test Set Admin ID COP";

	ClearControls();
	SetDeviceName("Test");

	GenerateAccessCode();
	AddAccessCodeToBuffer(adminAccessCode, 0);

	ClearInputBuffer();
	inputBuffer[0] = SetAdminIDOpCode;
	int newCount = AddAccessCodeToBuffer(inputBuffer, 1);
	inputBuffer[newCount++] = ADMIN_ID_SIZE;

	// Add admin ID to input buffer
	for(int i = 0; i < ADMIN_ID_SIZE; i++)
	{
		inputBuffer[newCount++] = i * 3;
	}

	ClearDeviceMemory();
	ExecuteControlOpCodes();

	ExpectedValue valueList[4];
	valueList[0].valueName = "Success Op Code";
	valueList[0].expectedValue = SuccessOpCode;
	valueList[0].actualValue = outputBuffer[0];

	valueList[1].valueName = "A for Admin";
	valueList[1].expectedValue = 'A';
	valueList[1].actualValue = outputBuffer[STANDARD_ID_SIZE+1+1];

	CheckResults(TestName, valueList, 2);
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
	TestMasterAccessCodeVerification();
	TestAccessCodeVerificationFailure();
	TestSetAdminIDOpCode();
}