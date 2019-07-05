#pragma once
#include "CFDriver.h"
extern "C"
{
#ifdef _WIN64



	__declspec(dllexport) void InitServer(int port);
	__declspec(dllexport) void InitClient(const char* adress, int port);
	__declspec(dllexport) void Disconnect();
	__declspec(dllexport) void RecvCommand(char & command);
	__declspec(dllexport) void InitSimulation();
	__declspec(dllexport) void StartSimulation();
	__declspec(dllexport) void StopSimulation();
	__declspec(dllexport) void ContinueSimulation();
	__declspec(dllexport) void SendGeometry(int len, float* geometry);
	__declspec(dllexport) void RecvGeometry(int len, float* geometry);
	__declspec(dllexport) void SendFlag(int len, int* flag);
	__declspec(dllexport) void RecvFlag(int len, int* flag);
	__declspec(dllexport) void SendVof(int len, float* vof);
	__declspec(dllexport) void RecvVof(int len, float* vof);
	__declspec(dllexport) void SendFile(const char* filename);
	__declspec(dllexport) void RecvFile();
	__declspec(dllexport) void SendData(int len, float* data);	//Backup function
	__declspec(dllexport) void RecvData(int len, float* data);	//Backup function
#endif

#ifdef linux
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
	extern void SendData(int len, double* data);	//Backup function
	extern void RecvData(int &len, double* data);	//Backup function
#endif

}