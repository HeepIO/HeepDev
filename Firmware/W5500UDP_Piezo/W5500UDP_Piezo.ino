#define ON_ARDUINO
#include "Heep_API.h"


#include <FastLED.h>
#define LED_PIN     11
#define NUM_LEDS    50
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

char controlName1 [] = "PLAY";
char controlName2 [] = "SLIDEFLUTE";
char controlName3 [] = "LED HUE";

char deviceName  [] = "RFID";

Control theControl1;
Control theControl2;
Control theControl3;


#define PIN 11

void setup()
{  
  Serial.begin(9600);

  Serial.println("START");
  
  pinMode(30, OUTPUT);
  pinMode(13, OUTPUT);
  
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);  
  
	SetupHeepDevice(deviceName);
  
  theControl1.controlName = controlName1;
  theControl1.controlID = 0;
  theControl1.controlDirection = 0;
  theControl1.controlType = 0;
  theControl1.highValue = 1;
  theControl1.lowValue = 0;
  theControl1.curValue = 0;
  AddControl(theControl1);

  
  theControl2.controlName = controlName2;
  theControl2.controlID = 1;
  theControl2.controlDirection = 0;
  theControl2.controlType = 1;
  theControl2.highValue = 100;
  theControl2.lowValue = 0;
  theControl2.curValue = 0;
  AddControl(theControl2);

  
  theControl3.controlName = controlName3;
  theControl3.controlID = 2;
  theControl3.controlDirection = 0;
  theControl3.controlType = 1;
  theControl3.highValue = 100;
  theControl3.lowValue = 0;
  theControl3.curValue = 0;
  AddControl(theControl3);
  
  FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  
	SetupHeepTasks();
	CreateInterruptServer();
}
unsigned long lastTime = 0;
void loop()
{
	  PerformHeepTasks();
    digitalWrite(13, GetControlValueByID(0));
    
    if(GetControlValueByID(1) < 90){
      tone(6,map(GetControlValueByID(1),100,0,110,880));
    }
    else{
      noTone(6);
    }
    if(millis() > lastTime + 10){
      writeLEDS(map(GetControlValueByID(2),0,100,0,255));
      lastTime = millis();
    }
}

void writeLEDS(byte WheelPos){
  
  for(int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(WheelPos,255,255);
  }
    FastLED.show();
}

