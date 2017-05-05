#include "Heep_API.h"

char controlName1 [] = "Switch";
char deviceName  [] = "Demo Wall Switch";

Control theControl1;

int rightSwitch = 3;

//#define CLEAR_MEMORY

void setup()
{
	Serial.begin(9600);
	delay(1000);
	Serial.println("START");

#ifdef CLEAR_MEMORY
	ClearMemory();
#else
	Serial.println("Reading Memory...");
	ReadMemory(deviceMemory, curFilledMemory);
#endif

	pinMode(rightSwitch, INPUT);
	digitalWrite(rightSwitch, HIGH);

	SetDeviceID(140001);
	SetDeviceName(deviceName);
	theControl1.controlName = controlName1;
	theControl1.controlID = 0;
	theControl1.controlDirection = 1;
	theControl1.controlType = 0;
	theControl1.highValue = 100;
	theControl1.lowValue = 0;
	theControl1.curValue = 0;
	AddControl(theControl1);

	SetupHeepTasks();
	CreateInterruptServer();
}

int stateLeft = 0;
int stateRight = 0;

void loop()
{
	CheckServerForInputs();
	PerformHeepTasks();

	if(digitalRead(rightSwitch) != stateRight)
	{
		stateRight = digitalRead(rightSwitch);
		SendOutputByID(0, stateRight);
		Serial.println("Sent Left");
	}
}