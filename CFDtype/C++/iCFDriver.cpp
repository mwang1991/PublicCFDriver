
#include "iCFDriver.h"

CFDriver s;
void InitServer(int port)
{
	s.init_server(port);

}

int InitClient(const char * adress, int port)
{
	return s.init_client(adress, port);
}

void Disconnect()
{
	printf("Disconnected\n");
	fflush(stdout);
	s.disconnect();

}


char RecvCommand()
{
	char command;
	s.recv_cmd(command);
	return command;
}

void SendCommand(char command)
{
	s.send_cmd(command);
}

void InitSimulation()
{
	printf("Sending command for simulation to initialize\n");
	fflush(stdout);
	s.send_cmd('0');

}

void StartSimulation()
{
	printf("Sending command for simulation to start\n");
	s.send_cmd('1');
}

void StopSimulation()
{
	printf("Sending command for simulation to stop\n");
	s.send_cmd('2');
}

void ContinueSimulation()
{
	printf("Sending command for simulation to continue\n");
	s.send_cmd('3');
}

void SendData(int len, char * data)
{
	s.send_data(data, len, 1);
}

int RecvData(char * data)
{
	int len;
	//s.send_cmd('D');
	s.recv_data(data, len, 1);
	return len;
}

void SendSize(int len) {
	s.send_data(&len, 4, 1);
}

int RecvSize() {
	int len;
	int temp;
	s.recv_data(&len, temp, 1);
	return len;
}

void SendGeometry(int len, float * geometry)
{
	s.send_data(geometry, len * 3, 4);	 //3D->x,y,z
}

void RecvGeometry(int len, float * geometry)
{
	s.send_cmd('G');	//ask for geometry data
	s.recv_data(geometry, len, 4);
	len = len / 3; //3D->x,y,z
}

void SendFlag(int len, int * flag)
{
	s.send_cmd('f');
	s.send_data(flag, len, 4);
	printf("Sending flags completed\n");
	fflush(stdout);
}

void RecvFlag(int len, int * flag)
{
	s.recv_data(flag, len, 4);
}

void SendVof(int len, float * vof)
{
	s.send_data(vof, len, 4);
}

void RecvVof(int len, float * vof)
{
	s.send_cmd('V');
	s.recv_data(vof, len, 4);
}

void SendFile(const char * filename)
{
	s.send_cmd('f');
	s.send_file(filename);
	
}

void RecvFile()
{
	s.recv_file();
}
