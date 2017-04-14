import json
from ControlValue import ControlValue
from Vertex import Vertex
from OutputData import OutputData
from DeviceMemory import DeviceMemory
from MemoryUtilities import MemoryUtilities
from CommonDataTypes import HeepIPAddress
from OpCodeUtilities import OpCodeUtilities

class Device:

	def __init__(self):
		self.ClientID = 12345678910
		self.IPAddress = HeepIPAddress(0, 0, 0, 0)
		self.ClientName = 'none'
		self.IconCustom = 0
		self.IconName = 'none'
		self.ControlList = []
		self.ControlQueue = []
		self.VertexList = []
		self.DeviceMemory = DeviceMemory()
		return

	def SetClientFrontEndXY(self, clientX, clientY) :
		self.DeviceMemory.SetClientXY(clientX, clientY, self.ClientID)

	def GetFrontEndXY(self) :
		return self.DeviceMemory.GetClientXY(self.ClientID)

	def SetClientName(self, clientName) :
		self.ClientName = clientName
		self.DeviceMemory.SetClientName(self.ClientName, self.ClientID)

	def DumpClientMemory(self) :
		self.DeviceMemory.WriteClientMemoryToFile()

 	def AddVertexByString(self, vertexStr) :
 		newVertex = Vertex()
 		newVertex.SetVertexFromString(vertexStr)
 		self.AddVertex(vertex)

 	def SetIconInformation(self, iconID, iconData) :
 		self.DeviceMemory.SetIconIDAndData(iconID, iconData, self.ClientID)

 	def AddVertex(self, vertex) :
 		self.VertexList.append(vertex)
 		self.DeviceMemory.SetVertex(vertex)

 	def DeleteVertex(self, vertex) :
 		for x in range(0, len(self.VertexList)) :
 			if vertex.IsVertexEqual(self.VertexList[x]) :
 				self.VertexList.remove(self.VertexList[x])
 				break

 		self.DeviceMemory.DeleteVertex(vertex)

 	def SetIPAddress(self, IPAddress) :
 		self.DeviceMemory.SetIPAddress(self.ClientID, IPAddress)

 	def GetIPAddress(self) :
 		return self.DeviceMemory.GetIPAddress(self.ClientID)

 	def RemoveVertex(self, destID, outputID, inputID) :

 		for x in range(0, len(self.VertexList)) :
 			if (destID 		== self.VertexList[x].destinationID 	and 
 				outputID 	== self.VertexList[x].outputID 			and  
 				inputID 	== self.VertexList[x].inputID			) :

 				self.VertexList.remove(self.VertexList[x])
 				return self.GetVerticesString()

		return self.GetVerticesString()

 	def QueueOutput(self, outID, value) :
 		outputCommandQueue = []

 		for x in range(0, len(self.VertexList)) :
 			if outID == self.VertexList[x].outputID :
 				myOutput = OutputData(self.VertexList[x].inputID, self.VertexList[x].destinationID, self.VertexList[x].destinationIP, self.VertexList[x].sourceID, value)
 				outputCommandQueue.append(myOutput)

 		return outputCommandQueue

 	def QueueControlByName(self, name, ControlValue) :
 		for x in range(0, len(self.ControlList)) :
 			if self.ControlList[x].ControlName == name :
 				self.ControlList[x].CurCtrlValue = ControlValue
 				self.QueueControl(self.ControlList[x])
 				return

 		return

	def QueueControl(self, ControlVal) :
		for x in range(0, len(self.ControlQueue) ) :
			if self.ControlQueue[x][0] == ControlVal.ControlName :
				self.ControlQueue[x] = (ControlVal.ControlName, ControlVal.CurCtrlValue)
				return

		self.ControlQueue.append( (ControlVal.ControlName, ControlVal.CurCtrlValue) )
		return

	def UpdateControlByName(self, name, value) :
		for x in range(0, len(self.ControlList)) :
			if self.ControlList[x].ControlName == name :
				self.ControlList[x].CurCtrlValue = value
				return
		return

	def UpdateControlsByString(self, controlString) :
		controlList = controlString.split(';')

		for x in range(0, len(controlList)) :
			if len(controlList[x]) > 0:
				curCommand = controlList[x].split(',')
				self.UpdateControlByName(curCommand[0], int(curCommand[1]))

		return

	def UpdateControlsByID(self, controlID, Data) :
		for x in range(0, len(self.ControlList)) :
			if self.ControlList[x].ControlID == controlID :
				self.ControlList[x].CurCtrlValue = Data
				return 0 # Return 0 for success

		return 1 # Meantime return 1 for not found or error

	def UpdatePositionByString(self, positionString) :
		updatedPosition = positionString.split(',')
		self.UpdatePositionByName( float(updatedPosition[1]), float(updatedPosition[2]))

		return

	def UpdatePositionByName(self, top, left) :
		self.Position.top = top
		self.Position.left = left
		return

	def GetQueuedControlString(self) :
		retString = ""
		for x in range(0, len(self.ControlQueue)) :
			retString = retString + self.ControlQueue[x][0] + ',' + str(self.ControlQueue[x][1]) +';'

		self.ControlQueue = []
		return retString	

	def GetClientString(self):
		
		byteArray = self.GetClientByteArray()
		return OpCodeUtilities().GetStringFromByteArray(byteArray)
		
	def GetClientByteArray(self) :
		byteArray = []
		byteArray = MemoryUtilities().AppendClientDataToByteArray(byteArray, self.ClientID)

		for x in range(0, len(self.ControlList)) :
			byteArray = MemoryUtilities().AppendControlDataToByteArray(byteArray, self.ClientID, self.ControlList[x])

		
		byteArray = MemoryUtilities().AppendByteArrayToByteArray(byteArray, self.DeviceMemory.miscMemory)
		return byteArray


	def SetClientFromString(self, clientString) :

		byteArray = OpCodeUtilities().ConvertStringToByteArray(clientString)
		self.ControlList = MemoryUtilities().GetClientControlValue(byteArray, self.ClientID)

	def GetVerticesString(self) :
		retStr = ""

		for x in range(0, len(self.VertexList)) :
			retStr = retStr + self.VertexList[x].GetVertexString()

		return retStr

	def SetVerticesFromString(self, vertexString) :
		self.VertexList = []
		splitStringSemiColon = vertexString.split(';')
		for x in range(0, len(splitStringSemiColon)) :
			if len(splitStringSemiColon[x]) > 0 :
				newVertex = Vertex()
				newVertex.SetVertexFromString(splitStringSemiColon[x])
				self.VertexList.append(newVertex)
		return

	def prepareForJSON(self) :
		self.ClientType = self.ClientType
		self.IPAddress = self.IPAddress
		self.ClientName = self.ClientName
		self.IconCustom = self.IconCustom
		self.IconName = self.IconName
		self.ControlList = self.ControlList
		self.ClientID = self.ClientID
		self.Position = self.Position

		for x in range(0, len(self.ControlList)) :
			self.ControlList[x].PrepareForJSONWrite()

		for x in range (0, len(self.VertexList)) :
			self.VertexList[x].PrepareForJSON()

	def toJSON(self):
		self.prepareForJSON()
		return json.dumps(self, default=lambda o: o.__dict__, sort_keys=True, indent=4, separators=(',', ': '))

	def toJSONDict(self) :
		return json.loads(self.toJSON())

	def fromJSON(self, Data) :
		self.fromDict(json.loads(Data))

	def fromDict(self, Dict) :
		self.__dict__ = Dict
		tempControlList = self.ControlList
		self.ControlList = []
		for x in range(0, len(tempControlList)) :
			newControl = ControlValue()
			newControl.FromDict(tempControlList[x])
			self.ControlList.append(newControl)

		tempVertexList = self.VertexList
		self.VertexList = []
		for x in range(0, len(tempVertexList)) :
			newVertex = Vertex()
			newVertex.FromDict(tempVertexList[x])
			self.VertexList.append(newVertex)
