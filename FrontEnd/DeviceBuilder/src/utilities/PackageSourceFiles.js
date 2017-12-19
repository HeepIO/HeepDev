import fileDownload from 'react-file-download'
import JSZip from 'jszip'
import randomNumber from 'random-number-csprng'

import ActionAndResponseOpCodes     from '../../../../Firmware/ServerlessFirmware/ActionAndResponseOpCodes.h'
import Arduino_EEPROM               from '../../../../Firmware/ServerlessFirmware/Arduino_EEPROM.h'
import Arduino_Timer                from '../../../../Firmware/ServerlessFirmware/Arduino_Timer.h'
import ArduinoW5100_HeepComms       from '../../../../Firmware/ServerlessFirmware/ArduinoW5100_HeepComms.h'
// import ArduinoW5500_HeepComms       from '../../../../Firmware/ServerlessFirmware/ArduinoW5500_HeepComms.h'
import CommonDataTypes              from '../../../../Firmware/ServerlessFirmware/CommonDataTypes.h'
import Device                       from '../../../../Firmware/ServerlessFirmware/Device.h'
import DeviceMemory                 from '../../../../Firmware/ServerlessFirmware/DeviceMemory.h'
import ENC28J60_HeepComms           from '../../../../Firmware/ServerlessFirmware/ENC28J60_HeepComms.h'
import ESP8266_HeepComms            from '../../../../Firmware/ServerlessFirmware/ESP8266_HeepComms.h'
import Heep_API                     from '../../../../Firmware/ServerlessFirmware/Heep_API.h'
import MemoryUtilities              from '../../../../Firmware/ServerlessFirmware/MemoryUtilities.h'
import PICW5500_HeepComms           from '../../../../Firmware/ServerlessFirmware/PICW5500_HeepComms.h'
import PICW5500_NonVolatileMemory   from '../../../../Firmware/ServerlessFirmware/PICW5500_NonVolatileMemory.h'
import PICW5500_Timer               from '../../../../Firmware/ServerlessFirmware/PICW5500_Timer.h'
import Scheduler                    from '../../../../Firmware/ServerlessFirmware/Scheduler.h'
import Simulation_HeepComms         from '../../../../Firmware/ServerlessFirmware/Simulation_HeepComms.h'
import Simulation_NonVolatileMemory from '../../../../Firmware/ServerlessFirmware/Simulation_NonVolatileMemory.h'
import Simulation_Timer             from '../../../../Firmware/ServerlessFirmware/Simulation_Timer.h'
import Socket_HeepComms             from '../../../../Firmware/ServerlessFirmware/Socket_HeepComms.h'

import { sys_phy_files } from '../utilities/SystemPHYCompatibilities'

var sourceFiles = {
  ActionAndResponseOpCodes: ActionAndResponseOpCodes,
  Arduino_EEPROM: Arduino_EEPROM,
  Arduino_Timer: Arduino_Timer,
  ArduinoW5100_HeepComms: ArduinoW5100_HeepComms,
  // ArduinoW5500_HeepComms: ArduinoW5500_HeepComms,
  CommonDataTypes: CommonDataTypes,
  Device: Device,
  DeviceMemory: DeviceMemory,
  ENC28J60_HeepComms: ENC28J60_HeepComms,
  ESP8266_HeepComms: ESP8266_HeepComms,
  Heep_API: Heep_API,
  MemoryUtilities: MemoryUtilities,
  PICW5500_HeepComms: PICW5500_HeepComms,
  PICW5500_NonVolatileMemory: PICW5500_NonVolatileMemory,
  PICW5500_Timer: PICW5500_Timer,
  Scheduler: Scheduler,
  Simulation_HeepComms: Simulation_HeepComms,
  Simulation_NonVolatileMemory: Simulation_NonVolatileMemory,
  Simulation_Timer: Simulation_Timer,
  Socket_HeepComms: Socket_HeepComms
}

export const packageSourceFiles = (deviceDetails, controls) => {

  console.log("Device: ", deviceDetails);
  console.log("Controls: ", controls);
  
  var zip = packageUniversalFiles();
  var autoGenIncludes = '';

  switch (deviceDetails.systemType) {

    case "Arduino" : 
      zip = packageArduinoFiles(deviceDetails, controls, zip);
      autoGenIncludes = getIncludes_Arduino(deviceDetails);
      break

    case "ESP8266" : 
      zip = packageESP8266Files(deviceDetails, controls, zip);
      autoGenIncludes = getIncludes_ESP8266(deviceDetails);
      break

    case "Linux" :

      zip.file('Socket_HeepComms.h', Socket_HeepComms);
      autoGenIncludes = getIncludes_Linux(deviceDetails);
      break

    case "PIC" :

      zip = packagePICFiles(deviceDetails, controls, zip);
      autoGenIncludes = getIncludes_PIC(deviceDetails);
      break

    case "Simulation" :

      zip = packageSimulationFiles(deviceDetails, controls, zip);
      autoGenIncludes = getIncludes_Simulation(deviceDetails);
      break

    default :
      console.log("Did not match systemType");
  }

  zip = getPHYforSys(deviceDetails.systemType, deviceDetails.physicalLayer, zip);

  var nameZip = deviceDetails.deviceName + '.zip';

  setIDAndMAC((deviceIDarray, MACAddressArray) => {

    zip.file('AutoGeneratedInfo.h', generateAutoInfoFile(deviceIDarray, MACAddressArray, autoGenIncludes));

    zip.generateAsync({type:"blob"})
    .then(function(content) {
        fileDownload(content, nameZip);
    });
  })
  

}

const packageUniversalFiles = () => {
  var zip = new JSZip();

  zip.file('ActionAndResponseOpCodes.h', ActionAndResponseOpCodes);
  zip.file('CommonDataTypes.h', CommonDataTypes);
  zip.file('Device.h', Device);
  zip.file('DeviceMemory.h', DeviceMemory);
  zip.file('Heep_API.h', Heep_API);
  zip.file('MemoryUtilities.h', MemoryUtilities);
  zip.file('Scheduler.h', Scheduler);

  return zip
}

const packageArduinoFiles = (deviceDetails, controls, zip) => {

  zip.file(deviceDetails.deviceName + ".ino", composeInoFile(deviceDetails, controls));

  zip.file('Arduino_EEPROM.h', Arduino_EEPROM);
  zip.file('Arduino_Timer.h', Arduino_Timer);

  return zip
}

const packageESP8266Files = (deviceDetails, controls, zip) => {
  zip.file(deviceDetails.deviceName + ".ino", composeInoFile(deviceDetails, controls));

  // zip.file('ESP8266_HeepComms.h', ESP8266_HeepComms);
  zip.file('Simulation_NonVolatileMemory.h', Simulation_NonVolatileMemory);
  zip.file('Arduino_Timer.h', Arduino_Timer);

  return zip
}

const packagePICFiles = (deviceDetails, controls, zip) => {

  zip.file('PICW5500_NonVolatileMemory.h', PICW5500_NonVolatileMemory);
  zip.file('PICW5500_Timer.h', PICW5500_Timer);

  return zip
}

const packageSimulationFiles = (deviceDetails, controls, zip) => {

  zip.file('Simulation_NonVolatileMemory.h', Simulation_NonVolatileMemory);
  zip.file('Simulation_Timer.h', Simulation_Timer);

  return zip
}

const getPHYforSys = (sys, phy, zip) => {

  var physicalLayerFilename = sys_phy_files[sys][phy];
  var sourceRef = physicalLayerFilename.split('.')[0];

  return zip.file(physicalLayerFilename, sourceFiles[sourceRef]);
}

const composeInoFile = (deviceDetails, controls) => {

    var fileContent = `
#include "Heep_API.h"

char deviceName [] = "` + deviceDetails.deviceName + `";\n\n`
+ initializeControls(controls)
+ createHardwareControlFunctionsArduinoSyntax(controls)
+ CreateHardwareReadFunctions(controls)
+ CreateHardwareWriteFunctions(controls)
+ `void setup()
{

  Serial.begin(115200);
  SetupHeepDevice(deviceName);\n\n  `  
+ setControls(controls)
+ `SetupHeepTasks();
  InitializeControlHardware();
  CreateInterruptServer(); 
}

void loop()
{
  PerformHeepTasks();\n`
+ GetReadWriteFunctionCalls(controls)
+ `\n}`

  return fileContent
}

var getPinDefineName = (control) => {
  var pinDefineStr = control.controlName.toUpperCase() + `_PIN`;
  return pinDefineStr;
}

var getPinDefine = (control) => {
  var pinDefineStr = `\n#define `+ getPinDefineName(control) + ` ` + control.pinNumber;
  return pinDefineStr;
}

var GetTabCharacter = () => {
  return `  `;
}

var GetReadFunctionName = (control) => {
  var ReadFunctionName = `Read` + control.controlName;
  return ReadFunctionName;
}

var GetWriteFunctionName = (control) => {
  var WriteFunctionName = `Write` + control.controlName;
  return WriteFunctionName;
}

var createHardwareControlFunctionsArduinoSyntax = (controls) => {
  var hardwareInitializations = `\nvoid InitializeControlHardware(){`;

  // output == 1, input == 0 
  // TODO: Make control direction into an enum with defined numbers just like Unity
  for (var i in controls) {
    var arduinoDirection = "OUTPUT";
    if(controls[i].controlDirection == 1){
      arduinoDirection = "INPUT";
    }

    hardwareInitializations += `\n` + GetTabCharacter() + `pinMode(` + getPinDefineName(controls[i]) + `,` + arduinoDirection + `);`;
  }

  hardwareInitializations += `\n}\n\n`;

  return hardwareInitializations;
}

// TODO: Make this function handle control types that are not just pin reads
var CreateHardwareReadFunctions = (controls) => {
  var hardwareReadFunctions = ``;

  // output == 1, input == 0 
  // TODO: Make control direction into an enum with defined numbers just like Unity
  for (var i in controls) {

    // Only react to outputs. Heep Outputs are Hardware Inputs
    if(controls[i].controlDirection == 1){
      hardwareReadFunctions += `int ` + GetReadFunctionName(controls[i]) + `(){\n`
        + GetTabCharacter() + `int currentSetting = digitalRead(` + getPinDefineName(controls[i]) + `);\n`
        + GetTabCharacter() + `SendOutputByID(` + controls[i].controlID + `,currentSetting);\n`
        + GetTabCharacter() + `return currentSetting;\n`
        + `}\n\n`;
    }
  }

  return hardwareReadFunctions;

}

// TODO: Make this function handle control types that are not just pin writes
var CreateHardwareWriteFunctions = (controls) => {
  var hardwareWriteFunctions = ``;

  // output == 1, input == 0 
  // TODO: Make control direction into an enum with defined numbers just like Unity
  for (var i in controls) {

    // Only react to inputs. Heep inputs are Hardware Outputs
    if(controls[i].controlDirection == 0){
      hardwareWriteFunctions += `int ` + GetWriteFunctionName(controls[i]) + `(){\n`
        + GetTabCharacter() + `int currentSetting = GetControlValueByID(` + controls[i].controlID + `);\n`
        + GetTabCharacter() + `digitalWrite(` + getPinDefineName(controls[i]) + `,currentSetting);\n`
        + GetTabCharacter() + `return currentSetting;\n`
        + `}\n\n`;
    }
  }

  return hardwareWriteFunctions;

}

var GetReadWriteFunctionCalls = (controls) => {
  var readWriteFunctions = ``;

  console.log("Enter readwrite function calls");

  // output == 1, input == 0 
  // TODO: Make control direction into an enum with defined numbers just like Unity
  for (var i in controls) {
    console.log("Readwrite " + i);

    if(controls[i].controlDirection == 1){
      readWriteFunctions += GetTabCharacter() + GetReadFunctionName(controls[i]) + `();`;
    }
    else{
      readWriteFunctions += GetTabCharacter() + GetWriteFunctionName(controls[i]) + `();`;
    }

    readWriteFunctions += `\n`;
  }

  console.log(readWriteFunctions);

  return readWriteFunctions;
}

const initializeControls = (controls) => {

  var controlDefs = ``;
  for (var i in controls) {
    controlDefs += `char controlName` + i + ` [] = "` + controls[i].controlName + `";\nControl control` + i + `;` + getPinDefine(controls[i]) + `\n\n`
  }

  return controlDefs

}

const setControls = (controls) => {
  var controlConfigs = ``;
  
  for (var i in controls) {
    controlConfigs += `control` + i + `.controlName = ` + `controlName` + i + `;\n  `;
    controlConfigs += `control` + i + `.controlID = ` + controls[i].controlID + `;\n  `;
    controlConfigs += `control` + i + `.controlDirection = ` + controls[i].controlDirection + `;\n  `;
    controlConfigs += `control` + i + `.controlType = ` + controls[i].controlType + `;\n  `;
    controlConfigs += `control` + i + `.highValue = ` + controls[i].highValue + `;\n  `;
    controlConfigs += `control` + i + `.lowValue = ` + controls[i].lowValue + `;\n  `;
    controlConfigs += `control` + i + `.curValue = ` + controls[i].curValue + `;\n  `;
    controlConfigs += `AddControl(control` + i + `);\n\n  `;
  }

  return controlConfigs
}

const setIDAndMAC = (launchDownloadCallback) => {

  var deviceIDArray = [];
  var MACAddressArray = [];

  randomNumber(0, 255)
  .then( (IDbyte1) => {

    deviceIDArray.push(IDbyte1);

    return randomNumber(0, 255)

  }).then((IDbyte2) => {

    deviceIDArray.push(IDbyte2);

    return randomNumber(0, 255);

  }).then((IDbyte3) => {

    deviceIDArray.push(IDbyte3);

    return randomNumber(0, 255);

  }).then((IDbyte4) => {
    
    deviceIDArray.push(IDbyte4);

    return randomNumber(0, 100);

  }).then((MACbyte1) => {
    
    MACAddressArray.push(MACbyte1);

    return randomNumber(0, 100);

  }).then((MACbyte2) => {
    
    MACAddressArray.push(MACbyte2);

    return randomNumber(0, 100);

  }).then((MACbyte3) => {
    
    MACAddressArray.push(MACbyte3);

    return randomNumber(0, 255);

  }).then((MACbyte4) => {
    
    MACAddressArray.push(MACbyte4);

    return randomNumber(0, 100);

  }).then((MACbyte5) => {
    
    MACAddressArray.push(MACbyte5);

    return randomNumber(0, 100);

  }).then((MACbyte6) => {
    
    MACAddressArray.push(MACbyte6);

    launchDownloadCallback(deviceIDArray, MACAddressArray);

  }).catch({code: "RandomGenerationError"}, (err) => {

    console.log("Something went wrong with promise chain...");

  });

}

const generateAutoInfoFile = (deviceIDarray, MACAddressArray, autoGenIncludes) => {

  var autoGenContent = `#pragma once

#define FIRMWARE_VERSION 1
#define FACTORY_RESET_PIN 4

#define MAX_MEMORY 255      // Bytes
#define NUM_VERTICES 100    // Vertex Pointers
#define NUM_CONTROLS 100    // Control Pointers
#define OUTPUT_BUFFER_SIZE 200  // Bytes
#define INPUT_BUFFER_SIZE 200 // Bytes

#define SYSTEM_TASK_INTERVAL 1000 // Time in ms
#define NUMBER_OF_TASKS 4

//#define USE_INDEXED_IDS
#define STANDARD_ID_SIZE 4

#ifdef USE_INDEXED_IDS
#define ID_SIZE 1
#else 
#define ID_SIZE STANDARD_ID_SIZE
#endif

// Include the standard Heep Data Types
#include "CommonDataTypes.h"\n\n`;

  autoGenContent += autoGenIncludes;

  autoGenContent += `

heepByte deviceIDByte [STANDARD_ID_SIZE] = {` + convertIntToHex(deviceIDarray) + `};
uint8_t mac[6] = {` + convertIntToHex(MACAddressArray) + `};\n`
  
  return autoGenContent
}

const convertIntToHex = (array) => {
  
  var commaString = '';

  for (var i in array) {
    commaString += '0x' + (array[i]).toString(16);

    if (i != array.length - 1) {
      commaString += ',';
    }
  }

  console.log("Hex Generated: ", commaString);

  return commaString
}

const getCommsFileName = (deviceDetails) => {
  var sys = deviceDetails.systemType;
  var phy = deviceDetails.physicalLayer;

  return sys_phy_files[sys][phy]
}

const getIncludes_Linux = (deviceDetails) => {
  return `#include "` + getCommsFileName(deviceDetails) + `"
#include "Simulation_NonVolatileMemory.h"
#include "Simulation_Timer.h"`
}

const getIncludes_ESP8266 = (deviceDetails) => {
  return `String SSID = "` + deviceDetails.ssid + `";
String Password = "` + deviceDetails.ssidPassword + `";
#include <string.h>
#include "` + getCommsFileName(deviceDetails) + `"
#include "Simulation_NonVolatileMemory.h"
#include "Arduino_Timer.h"`
}

const getIncludes_Arduino = (deviceDetails) => {
  return `#include <string.h>
#include "` + getCommsFileName(deviceDetails) + `"
#include "Arduino_EEPROM.h"
#include "Arduino_Timer.h"
`
}

const getIncludes_Simulation = (deviceDetails) => {
  return `#include <string.h>
#include "` + getCommsFileName(deviceDetails) + `"
#include "Simulation_NonVolatileMemory.h"
#include "Simulation_Timer.h"`
}

const getIncludes_PIC = (deviceDetails) => {
  return `#include "` + getCommsFileName(deviceDetails) + `"
#include "PICW5500_NonVolatileMemory.h"
#include "PICW5500_Timer.h"`
}

