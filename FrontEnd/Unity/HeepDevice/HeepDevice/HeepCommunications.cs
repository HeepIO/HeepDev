﻿using System;
using System.Net;  
using System.Net.Sockets;  
using System.Text;  
using System.Collections;
using System.Collections.Generic;

namespace Heep
{
	public class HeepCommunications
	{

		// Incoming data from the client.  
		public static string data = null;  

		public static void StartListening(HeepDevice device) {  
			// Data buffer for incoming data.  
			byte[] bytes = new Byte[1024];  

			byte [] IPAddrArray = {0, 0, 0, 0};
			IPAddress theAddr = new IPAddress(IPAddrArray);
			Console.WriteLine ("here");
			IPEndPoint localEndPoint = new IPEndPoint(theAddr, 5000);  
			Console.WriteLine ("here");

			// Create a TCP/IP socket.  
			Socket listener = new Socket(AddressFamily.InterNetwork,  
				SocketType.Stream, ProtocolType.Tcp );  
			Console.WriteLine ("here4");
			Console.WriteLine (theAddr.ToString ());

			// Bind the socket to the local endpoint and   
			// listen for incoming connections.  
			try {  
				listener.Bind(localEndPoint);  
				Console.WriteLine ("Frr");
				listener.Listen(10);  

				// Start listening for connections.  
				while (true) {  
					Console.WriteLine("Waiting for a connection...");  
					// Program is suspended while waiting for an incoming connection.  
					Socket handler = listener.Accept(); 

					Console.WriteLine("Data accepted");

					data = null;  

					bytes = new byte[1024];  
					int bytesRec = handler.Receive(bytes); 
					Console.WriteLine(bytesRec);
					data += Encoding.ASCII.GetString(bytes,0,bytesRec); 

					// Show the data on the console.  
					Console.WriteLine( "Text received : {0}", data);  

					List <byte> commandData = new List<byte> ();
					for(int i = 0; i < bytesRec; i++)
					{
						commandData.Add(bytes[i]);
					}

					// Echo the data back to the client.  
//					byte[] msg = Encoding.ASCII.GetBytes(data); 
					byte[] msg = new byte[200];
					List <byte> fromparser = HeepParser.ParseCommand(commandData, device);

					for(int i = 0; i < fromparser.Count; i++)
					{
						msg[i] = fromparser[i];
					}


					handler.Send(msg);  
					handler.Shutdown(SocketShutdown.Both);  
					handler.Close();  
				}  

			} catch (Exception e) {  
				Console.WriteLine(e.ToString());  
			}  

			Console.WriteLine("\nPress ENTER to continue...");  
			Console.Read();  

		}  

		public HeepCommunications ()
		{
		}
	}
}
