#include <iostream>
#include "ActionAndResponseOpCodes.h"

using namespace std;

struct ExpectedValue
{
	std::string valueName;
	int expectedValue;
	int actualValue;
};

void OnSuccess(std::string testName)
{
	cout << testName << ": " << "Passed" << endl;
}

void OnFailure(std::string testName)
{
	cout << testName << ": " << "Failed" << endl;
}

void ListExpected(ExpectedValue value)
{
	cout << "    " << value.valueName << " - Expected: " << value.expectedValue << " Found: " << value.actualValue << endl;
}

void CheckResults(std::string testName, ExpectedValue valueList [], int numberOfValues)
{
	for(int i = 0; i < numberOfValues; i++)
	{
		if(valueList[i].expectedValue != valueList[i].actualValue)
		{
			OnFailure(testName);

			for(int j = 0; j < numberOfValues; j++)
			{
				ListExpected(valueList[j]);
			}

			return;
		}
	}

	OnSuccess(testName);
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

void TestGetDeviceIDOctets()
{
	std::string TestName = "Add Char to Device Memory";

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

int main(void) 
{
	cout << "Begin Tests" << endl;

	TestAddCharToBuffer();
	TestAddCharToDeviceMemory();
	TestGetDeviceIDOctets();

	return 0;
}