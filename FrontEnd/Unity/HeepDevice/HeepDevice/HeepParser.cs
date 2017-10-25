﻿using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;  

namespace Heep
{
	public static class HeepParser
	{
		public static List<byte> ParseCommand(List <byte> commandBuffer, HeepDevice theDevice)
		{
			if (commandBuffer [0] == HeepLanguage.IsHeepDeviceOpCode) {
				return ParseIsHeepDeviceCommand (commandBuffer, theDevice);
			} else if (commandBuffer [0] == HeepLanguage.SetValueOpCode) {
				return ParseSetValueCommand (commandBuffer, theDevice);
			} else if (commandBuffer [0] == HeepLanguage.SetVertexOpCode) {
				return ParseSetVertexCommand (commandBuffer, theDevice);
			}

			return AddErrorMessageToBuffer ("Invalid Command", theDevice);
		}

		public static void ParseROP(List <byte> ROPBuffer)
		{
			if (ROPBuffer [0] == HeepLanguage.MemoryDumpOpCode) {
				Console.WriteLine ("MEMORY DUMP!!!");
				ParseMemoryDump (ROPBuffer);
			}
		}

		public static void ParseMemoryDump(List <byte> buffer)
		{
			int counter = 1;

			DeviceID newDeviceID = HeepLanguage.GetDeviceIDFromBuffer (buffer, counter);
			counter += newDeviceID.GetDeviceIDSize ();
			int numBytes = HeepLanguage.GetNumberFromBuffer (buffer, ref counter, 1);

			int firmwareVersion = 0;

			string deviceName = "";

			List <Control> controlList = new List<Control>();
			List <Vertex> vertexList = new List<Vertex>();

			while (counter < buffer.Count) {
				
				if (buffer [counter] == HeepLanguage.ClientDataOpCode) {
					
					firmwareVersion = parseClientDataOpCode (buffer, ref counter);
					Console.WriteLine ("Firmware Version: " + firmwareVersion);

				} else if (buffer [counter] == HeepLanguage.ControlOpCode) {

					parseControlOpCode (buffer, ref counter);
					//controlList.Add(newControl);

				} else {
					counter++;
				}
			
			}
		}

		public static int UnwrapMOPHeader(List <byte> buffer, ref int counter)
		{
			DeviceID newDeviceID = HeepLanguage.GetDeviceIDFromBuffer (buffer, counter);
			counter += newDeviceID.GetDeviceIDSize ();
			int numBytes = HeepLanguage.GetNumberFromBuffer (buffer, ref counter, 1);

			return numBytes;
		}

		public static void parseControlOpCode(List <byte> buffer, ref int counter)
		{
			//Control newControl = new Control();
			int numBytes = UnwrapMOPHeader (buffer, ref counter);
			int controlID = HeepLanguage.GetNumberFromBuffer (buffer, ref counter, 1);

			//return newControl;
		}

		public static int parseClientDataOpCode(List <byte> buffer, ref int counter)
		{

			int numBytes = UnwrapMOPHeader (buffer, ref counter);

			int firmwareversion = HeepLanguage.GetNumberFromBuffer (buffer, ref counter, 1);

			return firmwareversion;
		}

		public static List<byte> ParseIsHeepDeviceCommand(List <byte> commandBuffer, HeepDevice theDevice)
		{
			return theDevice.GetMemoryDump ();
		}

		public static List<byte> ParseSetValueCommand(List <byte> commandBuffer, HeepDevice theDevice)
		{
			int controlID = commandBuffer [2];
			int controlValue = commandBuffer [3];
			theDevice.SetControlByID (controlID, controlValue);

			return AddSuccessMessageToBuffer ("Successfully Set Value", theDevice);
		}

		public static List<byte> ParseSetVertexCommand(List <byte> commandBuffer, HeepDevice theDevice)
		{
			int counter = 1;
			int numBytes = HeepLanguage.GetNumberFromBuffer (commandBuffer, ref counter, 1);
			DeviceID txID = HeepLanguage.GetDeviceIDFromBuffer (commandBuffer, counter);
			counter += txID.GetDeviceIDSize ();
			DeviceID rxID = HeepLanguage.GetDeviceIDFromBuffer (commandBuffer, counter);
			counter += rxID.GetDeviceIDSize ();
			int txControl = HeepLanguage.GetNumberFromBuffer (commandBuffer, ref counter, 1);

			int rxControl = HeepLanguage.GetNumberFromBuffer (commandBuffer, ref counter, 1);

			IPAddress destIP = HeepLanguage.GetIPAddrFromBuffer (commandBuffer, counter);

			Vertex newVertex = new Vertex (rxID, txID, rxControl, txControl, destIP);

			theDevice.AddVertex (newVertex);

			return AddSuccessMessageToBuffer ("Vertex Set", theDevice);;
		}

		private static List<byte> AddSuccessMessageToBuffer(String message, HeepDevice theDevice)
		{
			List <byte> outputBuf = new List<byte>();

			outputBuf.Add (HeepLanguage.SuccessOpCode);
			HeepLanguage.AddDeviceIDToMemory (outputBuf, theDevice.GetDeviceID ());

			byte stringLength = (byte)message.Length;
			outputBuf.Add (stringLength);

			for (int i = 0; i < message.Length; i++) {
				outputBuf.Add ((byte)message [i]);
			}

			return outputBuf;
		}

		private static List<byte> AddErrorMessageToBuffer(String message, HeepDevice theDevice)
		{
			List <byte> outputBuf = new List<byte>();

			outputBuf.Add (HeepLanguage.ErrorOpCode);
			HeepLanguage.AddDeviceIDToMemory (outputBuf, theDevice.GetDeviceID ());

			byte stringLength = (byte)message.Length;
			outputBuf.Add (stringLength);

			for (int i = 0; i < message.Length; i++) {
				outputBuf.Add ((byte)message [i]);
			}

			return outputBuf;
		} 
			
	}
}

