#include "CFDtype.h"
void CFDtype::init()
{
	data = new char[datalen];
}

char* CFDtype::getData()
{
	return data;
}

void CFDtype::Recv()
{
	count = 0;
	char *buffer = new char[datalen];
	RecvData(buffer);
	FromCFData(buffer);
	delete buffer;
}

void CFDtype::Send()
{
	count = 0;
	ToCFData();
	SendData(datalen, data);
}

//send an array of CFDtype
void CFDtype::SendArray(CFDtype _array[])
{
	int size = sizeof(_array);
	SendSize(size);
	int _len = 0;
	int _count = 0;
	for (int i = 0; i < sizeof(_array); i++)
		_len += _array[i].datalen;
	char *_data = new char[_len];
	for (int i = 0; i < sizeof(_array); i++)
	{
		_array[i].count = 0;
		_array[i].ToCFData();
		strcpy(_data + _count, _array[i].getData());
		memcpy(_data + _count, _array[i].getData(), _array[i].datalen);
		_count += _array[i].datalen;
	}
	SendData(_len, _data);
	delete _data;
}


void CFDtype::RecvArray(std::vector<CFDtype*> _array)  //generate array of the type from recieved data
{

	//recieve data from server
	char *_data = new char[sizeof(_array) * _array[0]->datalen];
	RecvData(_data);

	//generate array
	int _count = 0;
	for (int i = 0; i < sizeof(_array); i++)
	{
		_array[i]->FromCFData(_data + _count);
		_count += _array[i]->datalen;
	}
	delete _data;
}

void CFDtype::Add(float _data)
{ 
	float* _buff = &_data;
	memcpy(data + count, _buff, 4);
	count += 4;
}

void CFDtype::Add(double _data)
{
	double* _buff = &_data;
	memcpy(data + count, _buff, 8);
	count += 8;
}

void CFDtype::Add(int _data)
{
	int* _buff = &_data;
	memcpy(data + count, _buff, 4);
	count += 4;
}

int CFDtype::GetInt(char _data[])
{
	int* _buff;
	sscanf(_data + count, "%4s", _buff);
	count += 4;
	return *_buff;
}

float CFDtype::GetFloat(char _data[])
{
	float* _buff;
	sscanf(_data + count, "%4s", _buff);
	count += 4;
	return *_buff;
}

double CFDtype::GetDouble(char _data[])
{
	double* _buff;
	sscanf(_data + count, "%8s", _buff);
	count += 8;
	return *_buff;
}


const char* DllString(std::string s)
{
	return s.c_str();
}




int initClient(std::string adress, int port = 8087)
{
	return InitClient(adress.c_str(), port);    //connect to server
}


void initServer(int port = 8087)
{
	InitServer(port);
}

void sendCommand(char c)
{
	SendCommand(c);
}
char recvCommand()
{
	return RecvCommand();
}
void disconnect()
{
	Disconnect();
}