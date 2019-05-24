using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.IO;
using UnityEngine;


public class CFDclient{


	Socket serverSocket; 
	IPEndPoint ipEnd; 
	string recvStr; 
	string sendStr; 
	byte[] recvData = new byte[1024]; 
	byte[] sendData = new byte[1024]; 
	///Length of received data
	int recvLen; 
	Thread connectThread; 
	int MAX_LEN = 1024;
	///True when sending data to stop the receiving thread.
	bool sending = false;

	///Initialize the connection to the server
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

	///send commande
	public void SocketSend(string sendStr)
	{
		sendData = new byte[1024];
		sendData = Encoding.ASCII.GetBytes(sendStr);
		serverSocket.Send(sendData, sendData.Length, SocketFlags.None);
		//Debug.Log ("gui");
	}
	///
	void SocketReceive()
	{
		SocketConnet ();
		ready = true;
		float []a = new float[1000];
		for (int i = 0; i < 1000; i++) {
			a [i] = i + i * 0.0001f;		
		}
		//send_data (a,"test");
		while (true)
		{
			while (!sending) {
				recvData = new byte[1024];
				//MonoBehaviour.print("Waiting for message");
				recvLen = serverSocket.Receive (recvData);
				MonoBehaviour.print (Encoding.ASCII.GetString (recvData));
				if (recvLen == 0) {
					SocketConnet ();
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
					ready = false;
					break;
				}
			}
		}
	}
	///Send float data
	public int send_data(float[] data, string name)
	{
		sending = true;
		int left = data.Length;
		MonoBehaviour.print ("Sending float data");
		SocketSend ("f");
		recvData = new byte[1024];
		serverSocket.Receive (recvData);
		//SocketSend ("987");
		serverSocket.Send(System.BitConverter.GetBytes(left), 4, SocketFlags.None);
		serverSocket.Receive (recvData);
		serverSocket.Send(Encoding.ASCII.GetBytes(name), name.Length, SocketFlags.None);
		serverSocket.Receive (recvData);

		byte[] buf = toByte (data);
		while (left > 256) {
			serverSocket.Send (Slice(buf,(data.Length - left) * 4, (data.Length - left + 256) * 4) , 1024, SocketFlags.None);
			left -= 256;
		}
		//MonoBehaviour.print("aaaaaaa");
		serverSocket.Send (Slice(buf,(data.Length - left) * 4 ,data.Length * 4 ) , left *4, SocketFlags.None);
		//MonoBehaviour.print("bbbbbb");
		MonoBehaviour.print ("All data sent");
		sending = false;
		return 0;
	}

	///Send integer data
	public int send_data(int[] data, string name)
	{
		sending = true;
		int left = data.Length;
		MonoBehaviour.print ("Sending integer data");
		SocketSend ("i");
		recvData = new byte[1024];
		serverSocket.Receive (recvData);
		//SocketSend ("987");
		serverSocket.Send(System.BitConverter.GetBytes(left), 4, SocketFlags.None);
		serverSocket.Receive (recvData);
		serverSocket.Send(Encoding.ASCII.GetBytes(name), name.Length, SocketFlags.None);
		serverSocket.Receive (recvData);

		byte[] buf = toByte (data);
		while (left > 256) {
			serverSocket.Send (Slice(buf,(data.Length - left) * 4, (data.Length - left + 256) * 4) , 1024, SocketFlags.None);
			left -= 256;
		}
		//MonoBehaviour.print("aaaaaaa");
		serverSocket.Send (Slice(buf,(data.Length - left) * 4 ,data.Length * 4 ) , left *4, SocketFlags.None);
		//MonoBehaviour.print("bbbbbb");
		MonoBehaviour.print ("All data sent");
		sending = false;
		return 0;
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

		SocketSend (".");
		recvLen = serverSocket.Receive(recvData);
		dataname = Encoding.ASCII.GetString(recvData);
		SocketSend (".");

		int[] intData = new int[left/4];
		float[] floatData = new float[left/4];
		while (left > 0) {
			recvData = new byte[1024];
			if (left < 1024) recvData = new byte[left];
			recvLen = serverSocket.Receive(recvData);
			switch (type) {
			case'i':
				MonoBehaviour.print ("Recieving integer data.");
				toInt(recvData).CopyTo (intData, count / 4);
				MonoBehaviour.print (toInt (recvData) [0]);
				//MonoBehaviour.print (toInt (recvData) [1]);
				//MonoBehaviour.print (toInt (recvData) [2]);
				break;
			case'f':
				MonoBehaviour.print ("Recieving float data.");
				toFloat (recvData).CopyTo (floatData, count / 4);
				MonoBehaviour.print (toFloat (recvData)[0]);
				//MonoBehaviour.print (toFloat (recvData) [1]);
				//MonoBehaviour.print (toFloat (recvData) [2]);
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

	///Disconnect
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
		

	int[] toInt(byte[] _data){
		int [] array = new int[_data.Length/4];
		for(int n = 0; n < _data.Length; n+=4)
		{
			int sample = System.BitConverter.ToInt32(_data, n);
			array [n / 4] = sample;
		}
		return array;
	}

	float[] toFloat(byte[] _data){
		float [] array = new float[_data.Length/4];
		for(int n = 0; n < _data.Length; n+=4)
		{
			float sample = System.BitConverter.ToSingle(_data, n);
			array [n / 4] = sample;
		}
		return array;
	}

	byte[] toByte(int[] _data){
		byte [] array = new byte[_data.Length * 4];
		for(int n = 0; n < _data.Length; n++)
		{
			byte[] sample = System.BitConverter.GetBytes(_data[n]);
			sample.CopyTo (array, n * 4);
		}

		return array;
	}

	byte[] toByte(float[] _data){
		byte [] array = new byte[_data.Length * 4];
		for(int n = 0; n < _data.Length; n++)
		{
			byte[] sample = System.BitConverter.GetBytes(_data[n]);
			sample.CopyTo (array, n * 4);
		}

		return array;
	}

	 byte[] Slice(byte[] source, int start, int end)
	{
		// Handles negative ends.
		if (end < 0)
		{
			end = source.Length + end;
		}
		int len = end - start;

		//MonoBehaviour.print(len);
		// Return new array.
		byte[] res = new byte[len];
		for (int i = 0; i < len; i++)
		{
			res[i] = source[i + start];
		}
		return res;
	}

}
