// iCFDriver.cpp : 定义 DLL 应用程序的导出函数。
//
#include "iCFDriver.h"
#ifdef WIN32
	#include "stdafx.h"
#endif

CFDriver s;

void InitServer(){
	s.init_server();
}
