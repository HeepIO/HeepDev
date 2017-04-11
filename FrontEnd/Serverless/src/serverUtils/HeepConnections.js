import net from 'net'
import os from 'os' 
import * as heepParser from './HeepMemoryParser'
import * as heepIconUtils from './HeepIconUtils'

var masterState = {
  clients: {clientArray: []},
  positions: {},
  controls: {controlStructure:{}},
  vertexList: {},
  icons: {},
  url: ''
};

var searchComplete = false;
var mostRecentSearch = {};

export var SearchForHeepDevices = () => {
  var gateway = findGateway();

  for (var i = 1; i <= 255; i++){
    var address = joinAddress(gateway,i)

    ConnectToHeepDevice(address, 5000, 'IsHeepDevice:');
  }
}

export var GetCurrentMasterState = () => {
  return masterState
}

export var ResetMasterState = () => {
  masterState = {
    clients: {clientArray: []},
    positions: {},
    controls: {controlStructure:{}},
    vertexList: {},
    icons: {},
    url: ''
  };

  return masterState
}

export var SendCommandToHeepDevice = (commandID, message) => {
  //SetVal:ControlName,Value
  //SetXY:X,Y
  var command = message.split(',');
  var clientID = command.shift();
  var sendMessage = commandID + ':' + command.join(',');

  var IPAddress = masterState.clients[clientID].IPAddress;
  console.log(sendMessage)
  ConnectToHeepDevice(IPAddress, 5000, sendMessage);
}

export var findGateway = () => {
  var networkInterfaces = os.networkInterfaces( );

  for (var interfaces in networkInterfaces) {
    for (var i = 0; i < networkInterfaces[interfaces].length; i++ ) {
      if (networkInterfaces[interfaces][i].netmask == '255.255.255.0'){
        var activeAddress = networkInterfaces[interfaces][i].address;
        var address = activeAddress.split('.');
        var myIp = address.pop();
        console.log('Searching on gateway: ', address);
        return address
      }
    }
  }
}

var joinAddress = (gateway, ip) => {
  return gateway.join('.') + '.' + ip.toString()
}

var ConnectToHeepDevice = (IPAddress, port, message) => {

  var sock = new net.Socket();
  sock.connect({host: IPAddress, port: port}, () => {
    sock.write(message);
  });

  sock.on('data', (data) => {
    console.log('Device found at address: ', IPAddress + ':' + port.toString());
    console.log(data.toString());
    console.log(data);

    mostRecentSearch[IPAddress] = true;
    var HeepChunks = heepParser.MemoryCrawler(data);

    if (HeepChunks != false){
      AddMemoryChunksToMasterState(HeepChunks, IPAddress)
    }
    sock.end();
  });

  sock.on('end', () => {});

  sock.on('error', () => {
    mostRecentSearch[IPAddress] = false;
  });
}

var AddMemoryChunksToMasterState = (heepChunks, IPAddress) => {
  console.log(heepChunks)
  for (var i = 0; i < heepChunks.length; i++) {
    if (heepChunks[i].op == 1){
      AddClient(heepChunks[i], IPAddress);

    } else if (heepChunks[i].op == 2){
      AddControl(heepChunks[i]);

    } else if (heepChunks[i].op == 3){
      
    } else if (heepChunks[i].op == 4){
      
    } else if (heepChunks[i].op == 5){
      
    } else if (heepChunks[i].op == 6){
      
    } else if (heepChunks[i].op == 7){
      SetClientPosition(heepChunks[i])
      
    } else if (heepChunks[i].op == 8){
      
    }
  }
}

var AddClient = (heepChunk, IPAddress) => {
  var clientID = heepChunk.clientID;
  var clientName = 'unset';
  var iconName = 'none';
  heepIconUtils.SetClientIconFromString(clientID, clientName, iconName);

  masterState.clients[clientID] = {
    ClientID: clientID,
    IPAddress: IPAddress,
    ClientName: clientName
  }

  if( masterState.clients.clientArray.indexOf(clientID) == -1){
    masterState.clients.clientArray.push(clientID);
  }

  SetNullPosition(clientID);
  masterState.controls.controlStructure[clientID] = {inputs: [], outputs: []};
  masterState.icons = heepIconUtils.GetIconContent();
}

var AddControl = (heepChunk) => {
  // Transition this to use new ControlID throughout frontend 
  var tempCtrlName = nameControlFromObject(heepChunk.clientID, heepChunk.control.ControlName) 
  masterState.controls[tempCtrlName] = heepChunk.control;
  masterState.controls[tempCtrlName].connectedControls = [];
  var currentIndex = SetControlStructure(heepChunk.clientID, tempCtrlName)

  masterState.positions[heepChunk.clientID][tempCtrlName] = SetControlPosition(heepChunk.clientID, currentIndex, heepChunk.control.ControlDirection);

}

var SetNullPosition = (clientID) => {
  
  var newPosition = {
    client: {top: 0, left: 0}
  }

  masterState.positions[clientID] = newPosition;
}

var SetClientPosition = (heepChunk) => {
  masterState.positions[heepChunk.clientID].client = heepChunk.position;
  RecalculateControlPositions(heepChunk.clientID);
}

var RecalculateControlPositions = (clientID) => {
  var startingPositions = masterState.positions[clientID];
  for (var controlName in startingPositions){
    if (controlName == 'client'){ 
      continue 
    }

    UpdateControlPosition(clientID, controlName);
  }
}

var UpdateControlPosition = (clientID, controlName) => {
  var clientPosition = masterState.positions[clientID].client;
  var thisPosition = masterState.positions[clientID][controlName];
  var direction = masterState.controls[controlName].ControlDirection;

  thisPosition.top = clientPosition['top'] + 45 + 1.5 + 25/2 + 55*(thisPosition.index), 
  thisPosition.left = direction == 0 ? clientPosition['left'] + 10 : clientPosition['left'] + 250;
  
}

var SetControlPosition = (clientID, index, direction) => {
  var clientPosition = masterState.positions[clientID]['client'];

  var position = {
    top: clientPosition['top'] + 45 + 1.5 + 25/2 + 55*index, 
    left: direction == 0 ? clientPosition['left'] + 10 : clientPosition['left'] + 250,
    index: index
  }
  
  return position;
}

var SetControlStructure = (clientID, controlID) => {

  if ( masterState.controls[controlID]['ControlDirection'] == 0){
    var inputs = masterState.controls.controlStructure[clientID].inputs;
    inputs.push(controlID);
    return inputs.length

  } else {
    var outputs = masterState.controls.controlStructure[clientID].outputs;
    outputs.push(controlID);
    return outputs.length

  }

}

var nameVertex = (vertex) => {
    return vertex['sourceID'] + '.' + vertex['outputName'] + '->' + vertex['destinationID'] + '.' + vertex['inputName'];
}

var nameControlFromObject = (clientID, controlName) => {
  return clientID +  '.' + controlName;
}
