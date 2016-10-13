import socket
from ControlValue import ControlValue
from PLCClient import PLCClient

class ServerConnection:

	sock = socket.socket()
	fileName = "clientList.tbl"
	TCP_PORT = 5000
	clientList = []
	host = ''
	backlog = 5 
	size = 1024 

	def __init__(self):
		return


	def GetDefaultGateway(self) :		
		ipv4 = ([(s.connect(('8.8.8.8', 53)), s.getsockname()[0], s.close()) 
				for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1].split('.'))
		TCP_STUB = '.'.join(ipv4[0:3]) + '.';
		return TCP_STUB

	def WriteClientList(self) :
		outFile = open(self.fileName, 'w')
		for x in range(0, len(self.clientList)) :
			outFile.write(self.clientList[x].GetClientString())
		outFile.close()

	def ReadClientList(self) :
		try :
			inFile = open(self.fileName, 'r')
			while 1:
				curStr = inFile.readline()

				if len(curStr) > 0 :
					stringList = curStr.split(',')
					if len(stringList) == 3 :
						newClient = PLCClient(int(stringList[1]), stringList[0], stringList[2], [])
						self.clientList.append(newClient)
				else :
					break
			inFile.close()
		except :
			print 'No client file found'
		return self.clientList

	def AddClientToList(self, newClient) :
		addClient = 1
		for x in range(0, len(self.clientList)) :
			if self.clientList[x].IPAddress == newClient.IPAddress :
				addClient = 0
				break

		if addClient :
			self.clientList.append(newClient)

		return self.clientList


	def ListenToNetwork(self) :

		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.bind((self.host, self.TCP_PORT)) 
		self.sock.listen(self.backlog)
		
		while 1: 
		    client, address = self.sock.accept() 
		    data = client.recv(self.size) 
		    print "Client Address: ", address[0]
		    controlList = []
		    controlList.append(ControlValue(100, 2, 'Slider1'))
		    controlList.append(ControlValue(200, 100, 'Slider2'))
		    newClient = PLCClient(0, address[0], 'TestClient', controlList)
		    self.AddClientToList(newClient)
		    self.WriteClientList()
		    print "Received Data: ", data
		    if data: 
		        client.send(data) 
		    client.close() 
