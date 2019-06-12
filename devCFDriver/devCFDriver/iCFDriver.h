#pragma once
#include "CFDriver.h"
extern "C"
{
	extern void InitServer(int port);
	extern void InitClient(const char* adress, int port);
	extern void RecvCommand(char & command);
	extern void InitSimulation();
	extern void StartSimulation();
	extern void StopSimulation();
	extern void ContinueSimulation();
	extern void SendGeometry(int len, float* geometry);
	extern void RecvGeometry(int len, float* geometry);
	extern void SendFlag(int len, int* flag);
	extern void RecvFlag(int len, int* flag);
	extern void SendVof(int len, float* vof);
	extern void RecvVof(int len, float* vof);
	extern void SendFile(const char* filename);
	extern void RecvFile();
	extern void SendData(int len, float* data);	//Backup function
	extern void RecvData(int len, float* data);	//Backup function

}