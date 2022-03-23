# PublicCFDriver
To facilitate the use of the library, an abstract class CFDtype is declared to create different structured data. 

Functions:
public static void initServer(int port = 8087): initials as server, the default port is 8087
public static int initClient(string address, int port = 8087): initials as client, default port is 8087
public static void sendCommand(char c): send a command with a single char
public static char recvCommand(): receive a command with a single char
The command should be managed by the user.
To transfer data between computers, functions like
public static void SendArray(CFDtype[] _array) : send an array of structured data
Declare a type of structured data inherited from CFDtype (eg. WindDirection):
Declare variables (float longitude; float latitude; float altitude; float windDirection;)
In the constructer, set the size of all variables (4 for int and float, 8 for double) and call init() at the end of the constructor.
Declare functions:
public override void FromCFData(byte[] _data): use GetFloat, GetInt… to extract data
public override byte[] ToCFData(): use add to add variables to the components to the data
public static WindDirection[] RecvArray(): generate array of structured data 
Use send/ recv for single data and SendArray/ RecvArray for an array of data

The basic functions are in the file CFDriver.cpp. Users calls functions in iCFDriver.cpp.
The library works on Linux and Win 64bit
For server, call InitServer(int port) with a number of port
For the client call InitClient(const char* adress, int port) to connect the server, with server’s address and port number.
The functions in iCFDriver must be imported as it is written in C++.


