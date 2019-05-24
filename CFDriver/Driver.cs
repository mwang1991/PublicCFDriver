using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.IO;
using UnityEngine;

namespace CFDriver
{
	public class CFDclient
	{
		Socket serverSocket; //客户端socket
		IPEndPoint ipEnd; //侦听端口
		string recvStr; //接收的字符串
		string sendStr; //发送的字符串
		byte[] recvData = new byte[1024]; //接收的数据，必须为字节
		byte[] sendData = new byte[1024]; //发送的数据，必须为字节
		int recvLen; //接收的数据长度
		Thread connectThread; //连接线程
		bool result = false;
		int MAX_LEN = 1024;

		//初始化
		public void InitSocket()
		{
			IPAddress ip = IPAddress.Parse("192.168.1.176");
			ipEnd = new IPEndPoint(ip, 5099);

			connectThread = new Thread(new ThreadStart(SocketReceive));
			connectThread.Start();
		}

		void SocketConnet()
		{
			if (serverSocket != null)
				serverSocket.Close();
			serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
			MonoBehaviour.print("ready to connect");
			serverSocket.Connect(ipEnd);
		}

		void SocketSend(string sendStr)
		{
			sendData = new byte[1024];
			sendData = Encoding.ASCII.GetBytes(sendStr);
			serverSocket.Send(sendData, sendData.Length, SocketFlags.None);
			//Debug.Log ("gui");
		}

		void SocketReceive()
		{
			SocketConnet ();
			sendtest ();
			while (true)
			{
				recvData = new byte[1024];
				//MonoBehaviour.print("Waiting for message");
				recvLen = serverSocket.Receive (recvData);
				//MonoBehaviour.print (Encoding.ASCII.GetString (recvData));
				if (recvLen == 0)
				{
					SocketConnet();
					continue;
				}
				switch (Encoding.ASCII.GetString (recvData) [0]) {
				case '#': 
					rec_file ();	//Receive a file
					break;
				case '*': 
					rec_data (Encoding.ASCII.GetString (recvData) [1]);	//Receive data
					break;
				case 'z':
					serverSocket.Close ();
					MonoBehaviour.print ("Disconnect");
					break;
				}

			}
		}

		int rec_data(char type){
			int iResult = 0;
			recvData = new byte[1024];
			string dataname;
			int left = 0;
			int count = 0;

			SocketSend(".");
			MonoBehaviour.print ("Ready to receive data.");

			recvLen = serverSocket.Receive(recvData);
			dataname = Encoding.ASCII.GetString (recvData);
			MonoBehaviour.print("Received data name: " + dataname);
			recvData = new byte[1024];

			SocketSend(".");
			recvLen = serverSocket.Receive(recvData);
			left = int.Parse(Encoding.ASCII.GetString (recvData));
			MonoBehaviour.print ("Received Data length is: " + left/4);

			serverSocket.Send(Encoding.ASCII.GetBytes("*" + type), 2, 0);

			int[] intData = new int[left/4];
			float[] floatData = new float[left/4];
			while (left > 0) {
				recvData = new byte[1024];
				if (left < 1024) recvData = new byte[left];
				recvLen = serverSocket.Receive(recvData);
				switch (type) {
				case'i':
					MonoBehaviour.print ("Recieving integer data.");
					(recvData).CopyTo (intData, count / 4);
					MonoBehaviour.print (byteToInt (recvData) [0]);
					//MonoBehaviour.print (byteToInt (recvData) [1]);
					//MonoBehaviour.print (byteToInt (recvData) [2]);
					break;
				case'f':
					MonoBehaviour.print ("Recieving float data.");
					byteToFloat (recvData).CopyTo (floatData, count / 4);
					MonoBehaviour.print (byteToFloat (recvData) [0]);
					//MonoBehaviour.print (byteToFloat (recvData) [1]);
					//MonoBehaviour.print (byteToFloat (recvData) [2]);
					break;
				default:
					{
						MonoBehaviour.print("Wrong data type");
						return -1;
					}
				}
				count += recvLen;
				left -= recvLen;
				//MonoBehaviour.print (left);
			}
			SocketSend(".");
			MonoBehaviour.print ("All data received.");
			return 0;
		}



		int rec_file() {
			int iResult = 0;
			recvData = new byte[1024];

			int filelength = 0;
			string filename;

			serverSocket.Send(Encoding.ASCII.GetBytes("#"), 1, 0);
			recvLen = serverSocket.Receive(recvData);
			filename= Encoding.ASCII.GetString(recvData);

			MonoBehaviour.print ( "Ready to receive a file. Reveived name:" + filename);

			//Receive file name
			int i = 0;

			MonoBehaviour.print ( "The file " + filename + " is to receive!\n" );

			//接收文件
			MonoBehaviour.print ("Ready to receive file!\n");
			serverSocket.Send(Encoding.ASCII.GetBytes("T"), 1, 0);

			if (iResult == 0 || iResult == -1)
			{
				MonoBehaviour.print ("Send file failed !\n\n\n" );
				return 0;
			}
			iResult  = serverSocket.Receive(recvData);
			MonoBehaviour.print( "The length of file to receive is "+ Encoding.ASCII.GetString(recvData) );
			int len = int.Parse(Encoding.ASCII.GetString (recvData));
			//filename = " aaa.txt";
			filename = filename.TrimEnd(filename[1023]);
			FileStream fs = File.Create(filename);
			//fs.Close();
			//rec_file.open(filename, ios::binary);
			int left  = len;
			//MonoBehaviour.print (left);
			while (left  > 0)
			{
				if (left < 1024) {
					byte[] recvLeft = new byte[left];

					iResult = serverSocket.Receive (recvLeft);
					//MonoBehaviour.print (left + "  " + iResult);
					fs.Write (recvLeft, 0, iResult);
					left -= iResult;
				} else {
					iResult = serverSocket.Receive (recvData);

					//cout  << "Receive partial data : " << iResult  << " bytes" << endl;

					fs.Write (recvData,0,iResult);
					left -= iResult;
				}
			}
			fs.Flush();
			fs.Close();

			if (left  == 0)
			{
				MonoBehaviour.print("Receive all the data\n");
			}
			return 1;
		}

		public void SocketQuit()
		{
			//先关闭客户端
			if (serverSocket != null)
				serverSocket.Close();
			//再关闭线程
			if (connectThread != null)
			{
				connectThread.Interrupt();
				connectThread.Abort();
			}
			//最后关闭服务器
			MonoBehaviour.print("diconnect");
		}

		void sendtest()
		{
			SocketSend ("f");
			recvData = new byte[1024];
			serverSocket.Receive (recvData);
			//SocketSend ("987");
			serverSocket.Send(System.BitConverter.GetBytes(3), 4, SocketFlags.None);
			serverSocket.Receive (recvData);
			float[] a = {5.5f,7.6f,8.54f};
			MonoBehaviour.print (byteToFloat (floatToByte (a)) [0]);
			serverSocket.Send(floatToByte(a), 12, SocketFlags.None);
		}

		int[] byteToInt(byte[] _data){
			int [] array = new int[_data.Length/4];
			for(int n = 0; n < _data.Length; n+=4)
			{
				int sample = System.BitConverter.ToInt32(_data, n);
				array [n / 4] = sample;
			}
			return array;
		}

		float[] byteToFloat(byte[] _data){
			float [] array = new float[_data.Length/4];
			for(int n = 0; n < _data.Length; n+=4)
			{
				float sample = System.BitConverter.ToSingle(_data, n);
				array [n / 4] = sample;
			}
			return array;
		}

		byte[] intToByte(int[] _data){
			byte [] array = new byte[_data.Length * 4];
			for(int n = 0; n < _data.Length; n++)
			{
				byte[] sample = System.BitConverter.GetBytes(_data[n]);
				sample.CopyTo (array, n * 4);
			}

			return array;
		}

		byte[] floatToByte(float[] _data){
			byte [] array = new byte[_data.Length * 4];
			for(int n = 0; n < _data.Length; n++)
			{
				byte[] sample = System.BitConverter.GetBytes(_data[n]);
				sample.CopyTo (array, n * 4);
			}

			return array;
		}
	}
}

