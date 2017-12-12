#ifndef ACTION_RESPONSE_OP_CODES_H
#define ACTION_RESPONSE_OP_CODES_H

#include "Device.h"

heepByte ackBuffer [RESEND_ACK_BYTES];

unsigned char outputBuffer [OUTPUT_BUFFER_SIZE];
unsigned int outputBufferLastByte = 0;

unsigned char inputBuffer [INPUT_BUFFER_SIZE];
unsigned int inputBufferLastByte = 0;

void ClearOutputBuffer()
{
	outputBufferLastByte = 0;
}

void ClearInputBuffer()
{
	inputBufferLastByte = 0;
}


// Ack consists of [TIME, RETRY_COUNT, OUTPUTBUFFER]
void ClearAckBuffer()
{
	// 0 will indicate that nothing is there since no COPs are 0, and time will not be 0. If time is 0, then the 
	// packet will receive an extra 10ms to live.
	for(int i = 0; i < RESEND_ACK_BYTES; i++)
	{
		ackBuffer[i] = 0;
	}
}

void DeleteAckDataAtIndex(int index)
{
	int localTracker = index;
	ackBuffer[localTracker] = 0; // Set time to 0
	localTracker++;
	ackBuffer[localTracker] = 0; // Set retry count to 0
	localTracker++;
	ackBuffer[localTracker] = 0; // Set OpCode to 0
	localTracker++;
	int numBytesToDelete = ackBuffer[localTracker];
	ackBuffer[localTracker] = 0; // Set numBytes to 0
	localTracker++;

	for( ; localTracker < index + numBytesToDelete + 4; localTracker++)
	{
		ackBuffer[localTracker] = 0;
	}

	for(int i = index; i < RESEND_ACK_BYTES; i++)
	{
		ackBuffer[i] = ackBuffer[localTracker];
		localTracker++;
	}
}

int GetNextOpenAckPositionPointer()
{
	int counter = 0;

	while(1)
	{
		if(ackBuffer[counter] == 0) // Time
		{
			return counter; 
		}

		counter++; // OpCode
		if(counter > RESEND_ACK_BYTES) return counter;

		// NEED TO ADD WHEN WE HAVE ACCESS CODES
		// counter += ACCESS_CODE_SIZE + 1;
		// if(counter > RESEND_ACK_BYTES) return counter;
		counter++;
		counter += ackBuffer[counter] + 1; // Get NumBytes
		if(counter > RESEND_ACK_BYTES) return counter;
	}

	return counter;
}

void ClearAckBufferFromIndexToEnd(int index)
{
	for(int i = index; i < RESEND_ACK_BYTES; i++)
	{
		ackBuffer[index] = 0;
	}
}

heepByte GetAdjustedTimeoutTime()
{
	return (heepByte)(GetMillis()/10);
}

void AddCurrentOutputBufferToAckBuffer()
{
	heepByte timeByte = GetAdjustedTimeoutTime();
	int firstIndex = GetNextOpenAckPositionPointer();

	if(firstIndex + outputBufferLastByte + 1 > RESEND_ACK_BYTES)
	{
		return; // FAILED FROM NOT ENOUGH MEMORY. HANDLE IN SOME WAY
	}

	ackBuffer[firstIndex] = timeByte;
	ackBuffer[firstIndex + 1] = 0; // Num retries starts at 0
	for(int i = 0; i < outputBufferLastByte; i++)
	{
		ackBuffer[firstIndex + i + 2] = outputBuffer[i];
	}
}

// Get the displacement between two byte values.
// Assume that value Left was recorded earlier than value Right
// Also assume that valueRight has not passed value left
heepByte GetByteValueDisplacementMovingRight(heepByte ValueLeft, heepByte valueRight)
{
	int displacement = 0;
	if(valueRight < ValueLeft)
	{
		displacement = (0xff+valueRight) - ValueLeft;
	}
	else 
	{
		displacement = valueRight - ValueLeft;
	}

	return (heepByte)displacement;
}

heepByte DidAckTimeout(heepByte ackStartTime)
{
	heepByte currentTime = GetAdjustedTimeoutTime();
	heepByte ackEndTime = (ackStartTime + ACK_TIMEOUT)%0xff;
	
	if(GetByteValueDisplacementMovingRight(currentTime, ackEndTime) > ACK_TIMEOUT)
		return 1;

	return 0;
}

void HandleAckBufferTimeouts()
{
	int counter = 0;

	while(1)
	{
		if(ackBuffer[counter] == 0) // Time
		{
			return; 
		}

		if(DidAckTimeout(ackBuffer[counter]))
		{
			// Respond to the timeout
		}

		counter++; // Retry Count
		if(counter > RESEND_ACK_BYTES) return;

		counter++; // OpCode
		if(counter > RESEND_ACK_BYTES) return;

		// NEED TO ADD WHEN WE HAVE ACCESS CODES
		// counter += ACCESS_CODE_SIZE + 1;
		// if(counter > RESEND_ACK_BYTES) return counter;
		counter++;
		counter += ackBuffer[counter] + 1; // Get NumBytes
		if(counter > RESEND_ACK_BYTES) return;
	}

}

void AddNewCharToOutputBuffer(unsigned char newMem)
{
	outputBufferLastByte = AddCharToBuffer(outputBuffer, outputBufferLastByte, newMem);
}

void AddDeviceIDToOutputBuffer_Byte(heepByte* deviceID)
{
	outputBufferLastByte = AddDeviceIDToBuffer_Byte(outputBuffer, deviceID, outputBufferLastByte);
}

void AddDeviceIDOrIndexToOutputBuffer_Byte(heepByte* deviceID)
{
	unsigned int counter = 0;
	heepByte copyDeviceID [STANDARD_ID_SIZE];
	CopyDeviceID(deviceID, copyDeviceID);
	GetIndexedDeviceID_Byte(copyDeviceID);
	AddBufferToBuffer(outputBuffer, copyDeviceID, ID_SIZE, &outputBufferLastByte, &counter);
}

unsigned long CalculateControlDataSize()
{
	unsigned long controlDataSize = 0;

	for(int i = 0; i < numberOfControls; i++)
	{
		controlDataSize += 12;
		controlDataSize += strlen(controlList[i].controlName);
	}

	return controlDataSize;
}

unsigned long CalculateCoreMemorySize()
{
	unsigned long coreMemorySize = 0;

	// Firmware Version / Initial Client ID will always be the same
	coreMemorySize += 7;

	return coreMemorySize + CalculateControlDataSize();
}

void FillOutputBufferWithSetValCOP(unsigned char controlID, unsigned char value)
{
	ClearOutputBuffer();
	AddNewCharToOutputBuffer(SetValueOpCode);
	AddNewCharToOutputBuffer(2);
	AddNewCharToOutputBuffer(controlID);
	AddNewCharToOutputBuffer(value);
}

// Updated
void FillOutputBufferWithControlData()
{
	for(int i = 0; i < numberOfControls; i++)
	{
		AddNewCharToOutputBuffer(ControlOpCode);
		AddDeviceIDOrIndexToOutputBuffer_Byte(deviceIDByte);
		unsigned int byteSize = strlen(controlList[i].controlName) + 6;
		AddNewCharToOutputBuffer(byteSize);
		AddNewCharToOutputBuffer(controlList[i].controlID);
		AddNewCharToOutputBuffer(controlList[i].controlType);
		AddNewCharToOutputBuffer(controlList[i].controlDirection);
		AddNewCharToOutputBuffer(controlList[i].lowValue);
		AddNewCharToOutputBuffer(controlList[i].highValue);
		AddNewCharToOutputBuffer(controlList[i].curValue);

		for(int j = 0; j < strlen(controlList[i].controlName); j++)
		{
			AddNewCharToOutputBuffer(controlList[i].controlName[j]);
		}
	}
}

// Updated
void FillOutputBufferWithDynamicMemorySize()
{
	AddNewCharToOutputBuffer(DynamicMemorySizeOpCode);
	AddDeviceIDOrIndexToOutputBuffer_Byte(deviceIDByte);
	AddNewCharToOutputBuffer(1);
	AddNewCharToOutputBuffer(MAX_MEMORY);
}

// Updated
void FillOutputBufferWithMemoryDump()
{
	ClearOutputBuffer();
	
	AddNewCharToOutputBuffer(MemoryDumpOpCode);
	AddDeviceIDToOutputBuffer_Byte(deviceIDByte);

	unsigned long totalMemory = curFilledMemory + CalculateCoreMemorySize() + 1;

	AddNewCharToOutputBuffer(totalMemory);

	// First data sent is control register so that receiver can decode the rest
	//AddNewCharToOutputBuffer(controlRegister);

	// Add Client Data
	AddNewCharToOutputBuffer(ClientDataOpCode);
	AddDeviceIDOrIndexToOutputBuffer_Byte(deviceIDByte);
	AddNewCharToOutputBuffer(1);
	AddNewCharToOutputBuffer(firmwareVersion);

	// Add Control Data
	FillOutputBufferWithControlData();

	// Add Dynamic Memory Size
	FillOutputBufferWithDynamicMemorySize();

	// Add Dynamic Memory
	for(int i = 0; i<curFilledMemory; i++)
	{
		AddNewCharToOutputBuffer(deviceMemory[i]);
	}
}

// Updated
void FillOutputBufferWithSuccess(char* message, int stringLength)
{
	ClearOutputBuffer();

	AddNewCharToOutputBuffer(SuccessOpCode);
	AddDeviceIDToOutputBuffer_Byte(deviceIDByte);

	unsigned long totalMemory = strlen(message);

	AddNewCharToOutputBuffer(totalMemory);

	for(int i = 0; i < totalMemory; i++)
	{
		AddNewCharToOutputBuffer(message[i]);
	}
}

// Updated
void FillOutputBufferWithError(char* message, int stringLength)
{
	ClearOutputBuffer();

	AddNewCharToOutputBuffer(ErrorOpCode);
	AddDeviceIDToOutputBuffer_Byte(deviceIDByte);

	unsigned long totalMemory = strlen(message);

	AddNewCharToOutputBuffer(totalMemory);

	for(int i = 0; i < totalMemory; i++)
	{
		AddNewCharToOutputBuffer(message[i]);
	}
}

void ExecuteMemoryDumpOpCode()
{
	FillOutputBufferWithMemoryDump();
}

void ExecuteSetValOpCode()
{
	unsigned int counter = 1;
	unsigned char numBytes = inputBuffer[counter++];
	unsigned char controlID = inputBuffer[counter++];
	unsigned int value = GetNumberFromBuffer(inputBuffer, &counter, numBytes - 1);

	int success = SetControlValueByIDFromNetwork(controlID, value);

	if(success == 0)
	{
		char SuccessMessage [] = "Value Set";
		FillOutputBufferWithSuccess(SuccessMessage, strlen(SuccessMessage));
	}
	else 
	{
		char ErrorMessage [] = "Failed to Set";
		FillOutputBufferWithError(ErrorMessage, strlen(ErrorMessage));
	}
}

// Updatded
void ExecuteSetPositionOpCode()
{
	unsigned int counter = 1;
	unsigned char numBytes = inputBuffer[counter++];
	unsigned int xValue = GetNumberFromBuffer(inputBuffer, &counter, 2);
	unsigned int yValue = GetNumberFromBuffer(inputBuffer, &counter, 2);

	UpdateXYInMemory_Byte(xValue, yValue, deviceIDByte);

	char SuccessMessage [] = "Value Set";
	FillOutputBufferWithSuccess(SuccessMessage, strlen(SuccessMessage));
}

// Updated
void ExecuteSetVertexOpCode()
{
	struct Vertex_Byte myVertex;

	unsigned int localCounter = 0;
	unsigned int counter = 1;
	unsigned char numBytes = GetNumberFromBuffer(inputBuffer, &counter, 1);
	AddBufferToBuffer(myVertex.txID, inputBuffer, STANDARD_ID_SIZE, &localCounter, &counter);
	localCounter = 0;
	AddBufferToBuffer(myVertex.rxID, inputBuffer, STANDARD_ID_SIZE, &localCounter, &counter);
	unsigned char txControl = GetNumberFromBuffer(inputBuffer, &counter, 1);
	unsigned char rxControl = GetNumberFromBuffer(inputBuffer, &counter, 1);

	struct HeepIPAddress vertexIP;
	vertexIP.Octet4 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	vertexIP.Octet3 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	vertexIP.Octet2 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	vertexIP.Octet1 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	myVertex.rxControlID = rxControl;
	myVertex.txControlID = txControl;
	myVertex.rxIPAddress = vertexIP;

	AddVertex(myVertex);

	ClearOutputBuffer();
	char SuccessMessage [] = "Vertex Set";
	FillOutputBufferWithSuccess(SuccessMessage, strlen(SuccessMessage));
}

// Updated
void ExecuteDeleteVertexOpCode()
{
	struct Vertex_Byte myVertex;

	unsigned int localCounter = 0;
	unsigned int counter = 1;
	unsigned char numBytes = GetNumberFromBuffer(inputBuffer, &counter, 1);
	AddBufferToBuffer(myVertex.txID, inputBuffer, STANDARD_ID_SIZE, &localCounter, &counter);
	localCounter = 0;
	AddBufferToBuffer(myVertex.rxID, inputBuffer, STANDARD_ID_SIZE, &localCounter, &counter);
	unsigned char txControl = GetNumberFromBuffer(inputBuffer, &counter, 1);
	unsigned char rxControl = GetNumberFromBuffer(inputBuffer, &counter, 1);

	struct HeepIPAddress vertexIP;
	vertexIP.Octet4 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	vertexIP.Octet3 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	vertexIP.Octet2 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	vertexIP.Octet1 = GetNumberFromBuffer(inputBuffer, &counter, 1);
	myVertex.rxControlID = rxControl;
	myVertex.txControlID = txControl;
	myVertex.rxIPAddress = vertexIP;

	if(DeleteVertex(myVertex) == 1)
	{
		ClearOutputBuffer();
		char errorMessage [] = "Failed to delete Vertex!";
		FillOutputBufferWithError(errorMessage, strlen(errorMessage));
	}
	else
	{
		ClearOutputBuffer();
		char SuccessMessage [] = "Vertex Deleted!";
		FillOutputBufferWithSuccess(SuccessMessage, strlen(SuccessMessage));
	}
}

// Updated
// Validate that a MOP can be added. Then restructure it for localIDs as necessary
int ValidateAndRestructureIncomingMOP(unsigned int MOPStartAddr, unsigned int* numBytes)
{
	if(*numBytes < STANDARD_ID_SIZE + 2)
	{
		return 1; // INVALID MOP
	}

	heepByte curID [STANDARD_ID_SIZE];
	MOPStartAddr++;
	unsigned int startID = MOPStartAddr;
	unsigned int localCounter = 0;
	AddBufferToBuffer(curID, inputBuffer, STANDARD_ID_SIZE, &localCounter, &MOPStartAddr);
	unsigned char bytesOfData = GetNumberFromBuffer(inputBuffer, &MOPStartAddr, 1);
	GetIndexedDeviceID_Byte(curID);

	int memDiff = STANDARD_ID_SIZE - ID_SIZE;
	*numBytes = *numBytes - memDiff;

	localCounter = 0;
	AddBufferToBuffer(inputBuffer, curID, ID_SIZE, &startID, &localCounter);
	startID = AddCharToBuffer(inputBuffer, startID, bytesOfData);

	for(int i = 0; i < bytesOfData; i++)
	{
		inputBuffer[startID + i] = inputBuffer[startID + i + memDiff];
	}

	return 0;
}

void ExecuteDeleteMOPOpCode()
{
	unsigned int counter = 1;

	unsigned int numBytes = GetNumberFromBuffer(inputBuffer, &counter, 1);
	int dataError = ValidateAndRestructureIncomingMOP(counter, &numBytes);

	if(dataError == 0)
	{
		unsigned int theFoundCode = 0;
		unsigned int deviceMemCounter = 0;
		int MOPSDeleted = 0;

		while(deviceMemCounter < curFilledMemory)
		{
			for(int i = 0; i < numBytes; i++)
			{
				if(deviceMemory[deviceMemCounter+i] != inputBuffer[counter+i])
				{
					break;
				}

				theFoundCode++;
			}

			if(theFoundCode == numBytes)
			{
				deviceMemory[deviceMemCounter] = FragmentOpCode;
				deviceMemory[deviceMemCounter + 5] = numBytes;
				MOPSDeleted++;
			}
			theFoundCode = 0;

			deviceMemCounter = SkipOpCode(deviceMemCounter);
		}

		if(MOPSDeleted > 0)
		{
			ClearOutputBuffer();
			char SuccessMessage [] = "MOP Deleted!";
			FillOutputBufferWithSuccess(SuccessMessage, strlen(SuccessMessage));
		}
		else
		{
			ClearOutputBuffer();
			char MyerrorMessage [] = "Cannot Delete: MOP not found";
			FillOutputBufferWithError(MyerrorMessage, strlen(MyerrorMessage));
		}
	}
	else
	{
		ClearOutputBuffer();
		char errorMessage [] = "Cannot Delete: Generic MOP was invalid!";
		FillOutputBufferWithError(errorMessage, strlen(errorMessage));
	}
}

void ExecuteAddMOPOpCode()
{
	unsigned int counter = 1;

	unsigned int numBytes = GetNumberFromBuffer(inputBuffer, &counter, 1);

	int dataError = ValidateAndRestructureIncomingMOP(counter, &numBytes);

	if(dataError == 0)
	{
		for(int i = 0; i < numBytes; i++)
		{
			AddNewCharToMemory(inputBuffer[counter]);
			counter++;
		}

		ClearOutputBuffer();
		char SuccessMessage [] = "MOP Added!";
		FillOutputBufferWithSuccess(SuccessMessage, strlen(SuccessMessage));
	}
	else
	{
		ClearOutputBuffer();
		char errorMessage [] = "Cannot Add: Delivered Generic MOP was determined to be invalid!";
		FillOutputBufferWithError(errorMessage, strlen(errorMessage));
	}

}

unsigned char IsROP()
{
	if(inputBuffer[0] == MemoryDumpOpCode 
		|| inputBuffer[0] == SuccessOpCode
		|| inputBuffer[0] == ErrorOpCode)
	{
		return 1;
	}

	return 0;
}

void ExecuteControlOpCodes()
{
	unsigned char ReceivedOpCode = inputBuffer[0];
	if(ReceivedOpCode == IsHeepDeviceOpCode)
	{
		ExecuteMemoryDumpOpCode();
	}
	else if(ReceivedOpCode == SetValueOpCode)
	{
		ExecuteSetValOpCode();
	}
	else if(ReceivedOpCode == SetPositionOpCode)
	{
		ExecuteSetPositionOpCode();
	}
	else if(ReceivedOpCode == SetVertexOpCode)
	{
		ExecuteSetVertexOpCode();
	}
	else if(ReceivedOpCode == DeleteVertexOpCode)
	{
		ExecuteDeleteVertexOpCode();
	}
	else if(ReceivedOpCode == AddMOPOpCode)
	{
		ExecuteAddMOPOpCode();
	}
	else if(ReceivedOpCode == DeleteMOPOpCode)
	{
		ExecuteDeleteMOPOpCode();
	}
	else
	{
		char errorMessage [] = "Invalid COP Received";
		FillOutputBufferWithError(errorMessage, strlen(errorMessage));
	}
}

#endif