#pragma once
#include "iCFDriver.h"
#include <vector>

class CFDtype
{
private:
	char* data;
	int count;

	virtual void ToCFData();	//generate length and add bytes from data
	virtual void FromCFData(char _data[]);		//read data from bytes
	

protected:
	int datalen;

	void init();
	void Add(float _data);
	void Add(double _data);
	void Add(int _data);
	int GetInt(char _data[]);
	float GetFloat(char _data[]);
	double GetDouble(char _data[]);

public:
	char* getData();
	void Recv();
	void Send();
	static void SendArray(CFDtype _array[]);	//send an array of CFDtype
	static void RecvArray(std::vector<CFDtype*> _array);  //generate array of the type from recieved data

};


const char* DllString(std::string s);
//static int* ToInt(char _data[]);
//char* ToByte(int _data[]);
//char* ToByte(float _data[]);
//static double* ToDouble(char _data[]);
int initClient(std::string adress, int port = 8087);
void initServer(int port = 8087);
void sendCommand(char c);
char recvCommand();
void disconnect();

