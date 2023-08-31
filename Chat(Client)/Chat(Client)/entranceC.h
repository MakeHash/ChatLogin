#pragma once
#include <string>
#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
namespace entranceC
{
	SOCKET SERVER;
	void GroupMSGsend( SOCKET SERVER, std::string login )
	{
		while ( true )
		{
			std::string log = login + ": ";
			std::string msg;
			std::getline( std::cin, msg );
			log += msg;
			UINT sz = log.size( );
			std::string szLg = std::to_string( sz );
			send( SERVER, szLg.c_str(), 5, NULL );
			send( SERVER, log.c_str(), sz, NULL );
			Sleep(3000);
		}
	}
	void GroupMSGrecv(LPVOID param)
	{
		while (true)
		{
			SOCKET yours = (int)param;
			char* msgSz = new char[5];
			recv(yours, msgSz, 5, NULL);
			UINT sz = atoi(msgSz);
			char* msg = new char[sz];
			recv(yours, msg, sz, NULL);
			std::string MSG(msg, sz);
			delete[] msg;
			delete[] msgSz;
			std::cout << MSG << "\n";
		}
	}
	void Registration( SOCKET yours )
	{
		std::cout << "Enter your login: ";
		std::string login;
		std::cin >> login;
		UINT sz = login.size();
		std::string sss = std::to_string(sz);
		send(yours, sss.c_str(), 3, NULL);
		send(yours, login.c_str(), sz, NULL);
		Sleep(100);
		std::cout << "Enter your password: ";
		std::string pasw;
		std::cin >> pasw;
		UINT size = pasw.size();
		std::string ppsW = std::to_string(size);
		send(yours, ppsW.c_str(), 3, NULL);
		send(yours, pasw.c_str(), size, NULL);
		system("cls");
		std::cout << "                                  [ ONLINE CHAT ]\n";
	}
	std::string LogIn(SOCKET yours)
	{
		std::cout << "Enter your login: ";
		std::string login;
		std::cin >> login;
		UINT sz = login.size();
		std::string ssss = std::to_string(sz);
		send(yours, ssss.c_str(), 3, NULL);
		send(yours, login.c_str(), sz, NULL);
		char* answ = new char[1];
		recv(yours, answ, 1, NULL);
		std::string buf(answ, 1);
		if (buf == "N")
		{
			delete[] answ;
			Registration(yours);
			return login;
		}
		else if (buf == "Y")
		{
			delete[] answ;
			bool pswAnsw = false;
			int count = 0;
			std::cout << "Enter your password: ";
			while (pswAnsw != true)
			{
				if (count == 3)
				{
				std::cout << "\n You've entered wrong password 3 times. Goodbye!\n";
				closesocket(yours);
				WSACleanup();
				exit(EXIT_FAILURE);
				}
				std::string psw;
				std::cin >> psw;
				UINT pswSZ = psw.size() ;
				std::string ppp = std::to_string(pswSZ);
				send(yours,ppp.c_str(), 5, NULL);
				send(yours, psw.c_str(), pswSZ, NULL);
				char* answ = new char[3];
				int rcv = 0;
				rcv  = recv(yours, answ, 3, NULL);
				if (rcv > 0)
				{
					std::string pswA(answ, 1);
					if (pswA == "Y")
					{
						delete[] answ;
						pswAnsw = true;
						break;
					}
					else
					{
						std::cout << "One more time: ";
						delete[] answ;
						++count;
					}
				}
			}
			if (pswAnsw == false)
			{
				closesocket(yours);
				WSACleanup();
				exit(EXIT_FAILURE);
			}
			char* srvScktsz = new char[5];
			recv(yours, srvScktsz, 5, 0);
			UINT srvSz = atoi( srvScktsz );
			char* server = new char[ srvSz ];
			recv(yours, server, srvSz, 0);
			std::string serverSocket( server, srvSz );
			SERVER = atoi(serverSocket.c_str());
			std::cout << SERVER;
			system("cls");
			std::cout << "                                  [ ONLINE CHAT ]\n";
			return login;
		}
	}
}