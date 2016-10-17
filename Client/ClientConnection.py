import socket
import time
from threading import Thread
import traceback

class ClientConnection:

	sock = socket.socket()
	serverIP = "none"
	fileName = "serverTable.tbl"
	TCP_PORT = 5000
	BUFFER_SIZE = 1500

	connectionAttempts = [0]*255
	threadList = []

	def __init__(self):
		return

	def GetServerFromFile(self) :
		myFile = open(self.fileName, 'r')
		ipStr = myFile.readline()
		myFile.close()
		if len(ipStr) > 0 :
			ipStr = ipStr.split(":")
			ipStr = ipStr[1].replace("\n", "")

		return ipStr

	def AttemptIPConnection(self, ipAddress, connectionTableAddr) :
		print ipAddress
		tempMessage = "Replace this message"
		try :
			self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			self.sock.settimeout(0.5)
			self.sock.connect((ipAddress, self.TCP_PORT))
			self.sock.send(tempMessage)
			data = self.sock.recv(self.BUFFER_SIZE)

			print ("received data:", data)
			if data == tempMessage :
				print ('Worked!')
				self.connectionAttempts[connectionTableAddr] = 1
		except : 
			print ('Failed')

	def GetDefaultGateway(self) :		
		ipv4 = ([(s.connect(('8.8.8.8', 53)), s.getsockname()[0], s.close()) 
				for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1].split('.'))
		TCP_STUB = '.'.join(ipv4[0:3]) + '.';
		return TCP_STUB

	def WriteServerIPToFile(self, ipAddress) :
		writeFile = open(self.fileName, 'w')
		writeFile.write("Server:")
		writeFile.write(ipAddress)
		writeFile.write('\n')
		writeFile.close()

	def WaitForThreadsToComplete(self) :
		return

	def SearchForServerIP(self) :
		TCP_STUB = self.GetDefaultGateway()

		for x in range(0, 254) :
			TCP_IP = TCP_STUB + str(x)
		
			try:
				time.sleep(0.01)
				t = Thread( target = self.AttemptIPConnection, args=(TCP_IP, x,) )
				t.start()
				self.threadList.append(2)
				print threadList
				#self.AttemptIPConnection(TCP_IP, x)
				
			except:
				print ('Failed to schedule search thread')

		time.sleep(2)

	def GetServerIPFromTable(self) :
		for x in range(0, len(self.connectionAttempts)) :
			if self.connectionAttempts[x] == 1 :
				return self.GetDefaultGateway() + str(x)

	def Connect(self) :
		# First Check for File and try to connect
		ipString = 'None' 
		try :
			ipString = self.GetServerFromFile()
			self.AttemptIPConnection(ipString)
			return
		except :
			self.sock.close()
			print ('Failed to connect via file')

		try :
			self.SearchForServerIP()
			ipString = self.GetServerIPFromTable()
			self.WriteServerIPToFile(ipString)

			print ('Server IP Found at: ', ipString)

		except :
			self.sock.close()
			print ('Failed to connect to a network')
		return


