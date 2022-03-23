using System.Runtime.InteropServices;
using System.Linq;


public abstract class CFDtype
{
	[DllImport("iCFDriver")] private static extern int RecvData(ref byte data);
	[DllImport("iCFDriver")] private static extern void SendData(int len, ref byte data);
	[DllImport("iCFDriver")] private static extern void SendSize(int len);
	[DllImport("iCFDriver")] protected static extern int RecvSize();
	[DllImport("iCFDriver")] private static extern int InitClient(ref byte adress, int port);
	[DllImport("iCFDriver")] private static extern void InitServer(int port);
	[DllImport("iCFDriver")] private static extern char RecvCommand();
	[DllImport("iCFDriver")] private static extern void SendCommand(char command);
	[DllImport("iCFDriver")] private static extern void Disconnect();


	protected int datalen;
	private byte[] data;
	private int count;


	//return byte of an object


	//generate length and add bytes from data
	protected abstract void ToCFData();

	//read data from bytes
	protected abstract void FromCFData(byte[] _data);

	//initialize data
	protected void init()
	{
		data = new byte[datalen]; 
	}

	public byte[] getData()
	{
		return data;
	}

	public void Recv()
    {
		count = 0;
		byte[] buffer = new byte[datalen];
		RecvData(ref buffer[0]);
		FromCFData(buffer);
	}

	public void Send()
    {
    	count = 0;
    	ToCFData();
		SendData(datalen, ref data[0]);
	}

	//send an array of CFDtype
	public static void SendArray(CFDtype[] _array)
	{
		int size = _array.Length;
		SendSize(size);
		int _len = 0;
		byte[] _data;
		int _count = 0;
		for (int i = 0; i < _array.Length; i++)
			_len += _array[i].datalen;
		_data = new byte[_len];
		for (int i = 0; i < _array.Length; i++)
		{
			_array[i].count = 0;
			_array[i].ToCFData();
			_array[i].getData().CopyTo(_data, _count);
			_count += _array[i].datalen;
		}
		SendData(_len, ref _data[0]);

	}


	public static void RecvArray(CFDtype[] _array)  //generate array of the type from recieved data
	{

		//recieve data from server
		byte[] _data = new byte[_array.Length * _array[0].datalen];
		RecvData(ref _data[0]);

		//generate array
		int _count = 0;
		for (int i = 0; i < _array.Length; i++)
		{
			_array[i].FromCFData(_data.Skip(_count).Take(_array[i].datalen).ToArray());
			_count += _array[i].datalen;

		}

	}

		protected void Add(float _data)
	{
		byte[] sample = System.BitConverter.GetBytes(_data);
		sample.CopyTo(data, count);
		count += 4;
	}

	protected void Add(double _data)
	{
		byte[] sample = System.BitConverter.GetBytes(_data);
		sample.CopyTo(data, count);
		count += 8;
	}

	protected void Add(int _data)
	{
		byte[] sample = System.BitConverter.GetBytes(_data);
		sample.CopyTo(data, count);
		count += 4;
	}

	protected int GetInt(ref byte[] _data)
	{
		count += 4;
		return System.BitConverter.ToInt32(_data, count - 4);
	}

	protected float GetFloat(ref byte[] _data)
	{
		count += 4;
		return System.BitConverter.ToSingle(_data, count - 4);
	}

	protected double GetDouble(ref byte[] _data)
	{
		count += 8;
		return System.BitConverter.ToDouble(_data, count - 8);
	}

	public static byte[] DllString(string s)
	{
		return System.Text.Encoding.ASCII.GetBytes(s);
	}

	public static int[] ToInt(byte[] _data)
	{
		int[] array = new int[_data.Length / 4];
		for (int n = 0; n < _data.Length; n += 4)
		{
			int sample = System.BitConverter.ToInt32(_data, n);
			array[n / 4] = sample;
		}
		return array;
	}

	public static byte[] ToByte(int[] _data)
	{
		byte[] array = new byte[_data.Length * 4];
		for (int n = 0; n < _data.Length; n++)
		{
			byte[] sample = System.BitConverter.GetBytes(_data[n]);
			sample.CopyTo(array, n * 4);
		}

		return array;
	}

	public static byte[] ToByte(float[] _data)
	{
		byte[] array = new byte[_data.Length * 4];
		for (int n = 0; n < _data.Length; n++)
		{
			byte[] sample = System.BitConverter.GetBytes(_data[n]);
			sample.CopyTo(array, n * 4);
		}

		return array;
	}


	public static double[] ToDouble(byte[] _data)
	{
		double[] array = new double[_data.Length / 8];
		for (int n = 0; n < _data.Length; n += 8)
		{
			double sample = System.BitConverter.ToDouble(_data, n);
			array[n / 8] = sample;
		}
		return array;
	}


	public static int initClient(string adress, int port = 8087)
	{
		return InitClient(ref DllString(adress)[0], port);    //connect to server
	}


	public static void initServer(int port = 8087)
	{
		InitServer(port);
	}

	public static void sendCommand(char c)
	{
		SendCommand(c);
	}
	public static char recvCommand()
	{
		return RecvCommand();
	}
	public static void disconnect()
    {
		Disconnect();
    }
}
