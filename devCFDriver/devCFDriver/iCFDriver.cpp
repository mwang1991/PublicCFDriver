#include "pch.h"

#include "iCFDriver.h"

CFDriver s;
void InitServer(int port)
{
	s.init_server(port);

}

void InitClient(const char * adress, int port)
{
	s.init_client(adress, port);
}

