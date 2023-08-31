#pragma once
#include <WinSock2.h>
#include <string>
namespace msg
{
	void SendMsg(SOCKET yours,std::string msg)
	{
		int bytesSend = msg.size( );
		std::string bytes = std::to_string( bytesSend );
		send (yours, bytes.c_str(), 5, NULL );
		send( yours, msg.c_str(), bytesSend, NULL );
	}
	std::string RecieveMsg(SOCKET yours)    
	{
		char* DataSize = new char[ 5 ];
		recv(yours, DataSize, 5, NULL);
		UINT sz = atoi(DataSize);  //HERE MAY BE SOME PROBLEMS
		char* msg = new char[ sz ];
		recv(yours, msg, sz+2, NULL);
		std::string message( msg );
		delete[] DataSize;
		delete[] msg;
		return message;
	}

}