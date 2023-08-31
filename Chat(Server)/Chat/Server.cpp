#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")

#include "entranceS.h"
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <pqxx/pqxx>
#include <algorithm>
#include <vector>
SOCKET server;//Server sockeT
std::vector<SOCKET> ClientCount;
fd_set fr, fw, fe;
timeval tv;
int main()
{
	//Startup
	WSACleanup();
	WSADATA data;
	WORD version = MAKEWORD(2, 1);
	int res = WSAStartup(version, &data);
	if (res == WSASYSNOTREADY || res == WSAEPROCLIM)//Проверка успешности установки взаимодействий в системе  
	{                                              // WSASYSNOTREADY[СИСТЕМА НЕ ГОТОВА] || WSAEPROCLIM[Лимит процессов в виндовс]
		std::cout << "WSAStartup FAILED: " << WSAGetLastError();
		return 1;
	}

	//Creating server socket
	server = INVALID_SOCKET;
	server = socket(AF_INET, SOCK_STREAM, NULL);
	if (server == INVALID_SOCKET)
	{
		std::cout << "[ SERVER SOCKET CREATION ERROR: " << WSAGetLastError() << " ]\n";
		closesocket(server);
		WSACleanup();
		return 1;
	}

	//Enter servers data
	sockaddr_in srvADDR;
	srvADDR.sin_addr.s_addr = inet_addr("127.0.0.1");
	srvADDR.sin_family = AF_INET;
	srvADDR.sin_port = htons(9001);

	//Bind socket to address
	res = 0;
	res = bind(server, (sockaddr*)&srvADDR, sizeof(sockaddr));
	if (res < 0)
	{
		std::cout << "[ BIND FAILURE: " << WSAGetLastError() << " ]\n";
		closesocket(server);
		WSACleanup();
		return 1;
	}

	//Start listening
	res = listen(server, 5);
	if (res < 0)
	{
		std::cout << "[ LISTENING FAILURE: " << WSAGetLastError() << " ]\n";
		closesocket(server);
		WSACleanup();
		return 1;
	}
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while (true)
	{
		FD_ZERO(&fr);

		FD_SET(server, &fr);
		res = select(0, &fr, nullptr, nullptr, &tv);
		if (res > 0)
		{
				if (FD_ISSET(server, &fr))
				{
					int szAddr = sizeof(sockaddr);
					SOCKET Client = INVALID_SOCKET;
					Client = accept(server, (sockaddr*)&srvADDR, &szAddr);
					if (Client == INVALID_SOCKET)
					{
						std::cout << "Cannot connect new socket " << WSAGetLastError() << "\n";
						closesocket(Client);
					}
					std::cout << "New Client!\n";
					entranceS::LogInS(server, Client, ClientCount);
				}
		}
	}
}
