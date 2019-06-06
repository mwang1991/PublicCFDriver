#pragma once
#include "CFDriver.h"
extern "C"
{
	extern void InitServer(int port);
	extern void InitClient(const char* adress, int port);
}