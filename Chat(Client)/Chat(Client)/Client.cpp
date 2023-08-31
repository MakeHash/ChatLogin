#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include <WinSock2.h>
#include <string>
#include <iostream>
#include "entranceC.h"


SOCKET Client = INVALID_SOCKET;

int main()
{
	WSACleanup( );
	WSADATA data;
	WORD version = MAKEWORD( 2, 1 );
	int res = WSAStartup( version, &data );
	if ( res == WSASYSNOTREADY || res == WSAEPROCLIM )//Проверка успешности установки взаимодействий в системе  
	{                                              // WSASYSNOTREADY[СИСТЕМА НЕ ГОТОВА] || WSAEPROCLIM[Лимит процессов в виндовс]
		std::cout << "WSAStartup FAILED: " << WSAGetLastError( );
		WSACleanup( );
		return -1;
	}
	Client = socket( AF_INET, SOCK_STREAM, NULL );
	if ( Client == INVALID_SOCKET )
	{
		std::cout << "SOCKET CAN'T BE CREATED!\n ERROR CODE: " << WSAGetLastError( ) << "\n";
		WSACleanup( );
		closesocket( Client );
		exit( EXIT_FAILURE );
	}
	std::cout << "SOCKET SUCCESFULLY CREATED!\n";
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons( 9001 );
	serv.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	memset( &serv.sin_zero, 0, 8 );
	int servsz = sizeof( sockaddr );
	res = connect( Client, ( struct sockaddr* )&serv, servsz );
	if ( res < 0 )
	{
		std::cout << "FAILED TO CONNECT! ";
		closesocket( Client );
		WSACleanup( );
		return 1;
	}
	else
	{
		std::cout << "connected!\n";
		std::string login = entranceC::LogIn( Client );
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&entranceC::GroupMSGrecv, (LPVOID)Client, NULL, NULL );
		entranceC::GroupMSGsend( entranceC::SERVER, login );
	}
}