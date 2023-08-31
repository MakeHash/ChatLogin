#pragma once
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <pqxx/pqxx>
#include <vector>
#pragma comment(lib, "Ws2_32.lib")
namespace entranceS
{

	struct CheckInDB
	{
		std::string passw;
		std::string username;
		std::string sckt;
	};

	void InsertInDB( SOCKET client, std::string username, std::string password )
	{
		try
		{
			std::string sckt = std::to_string( client );
			pqxx::connection Cnct("host=localhost port=5432 dbname=postgres user=postgres password=123456789");
			pqxx::work user(Cnct);
			std::string sql = "INSERT INTO onlinechat(username,password,socket) "
				"VALUES('" + username + "','" + password + "','" + sckt + "');";
			user.exec( sql );
			user.commit( );
			std::cout << "Records Created Successfuly!\n";
		}
		catch ( std::exception& e )
		{
			std::cerr << "INSERTING FAILED: " << e.what( ) << "\n";
		}
	}

	CheckInDB SearchInDB( std::string user )
	{
		CheckInDB obj;
		try
		{
			pqxx::connection Cnct("host=localhost port=5432 dbname=postgres user=postgres password=123456789");
			const char* sql;
			sql = "SELECT * FROM onlinechat";
			pqxx::nontransaction srchDB( Cnct );
			obj.passw = "ERROR";
			obj.username = "ERROR";
			obj.sckt = "ERROR";
			pqxx::result R( srchDB.exec( sql ) );
			for ( pqxx::result::iterator it = R.begin(); it < R.end( ); ++it )
			{
				if (it[ 0 ].as< std::string >( ) == user)
				{
					std::cout << "FIND!\n";
					obj.passw = it[ 1 ].as< std::string>( );
					obj.username = it[ 0 ].as<std::string>( );
					obj.sckt = it[ 2 ].as< std::string >( );
				}
			}
			srchDB.commit( );
		}
		catch ( std::exception& e )
		{
			std::cerr << "\nSearch In DB ERROR: " << e.what( ) << "\n";
		}
		return obj;
	}



	void RegS(SOCKET server, SOCKET client, std::vector<SOCKET> clntVec)
	{
		//Login
		char* logAnsw = new char[5];
		int rec = 0;
		rec = recv(client, logAnsw, 5, NULL);
		if (rec > 0)
		{
			UINT szLog = atoi(logAnsw);
			char* log = new char[szLog];
			recv(client, log, szLog, NULL);
			std::string login(log,szLog);
			delete[] logAnsw;
			delete[] log;
			//Password
			char* paswSz = new char[5];
			rec = 0;
			rec = recv(client, paswSz, 5, NULL);
			if (rec > 0)
			{
				UINT pswSz = atoi( paswSz );
				char* pasw = new char[ pswSz ];
				recv( client , pasw, pswSz, NULL );
				std::string password( pasw, pswSz );
				delete[] paswSz;
				delete[] pasw;

				//Inserting in DataBase 
				InsertInDB( client, login, password );
				clntVec.push_back( client );
			}
		}
	}

	void LogInS( SOCKET server, SOCKET client,std::vector<SOCKET> clntVec )
	{
		char* logAnsw = new char[ 5 ];
		int rcv = 0;
		rcv = recv(client, logAnsw, 5, NULL);
		if (rcv > 0)
		{
			UINT szLog = atoi(logAnsw);
			std::cout << szLog <<"\n";
			char* log = new char[szLog];
			rcv = 0;
			rcv = recv(client, log, szLog, NULL);
			if (rcv > 0)
			{
				std::string check(log, szLog);
				std::cout << check << "\n";
				entranceS::CheckInDB db;
				db = entranceS::SearchInDB(check);
				if (db.passw == "ERROR")
				{
					std::string answ = "N";
					delete[] logAnsw;
					delete[] log;
					send(client, answ.c_str(), 3, NULL);
					RegS(server, client, clntVec);
				}
				else
				{
					std::string answ = "Y";
					delete[] logAnsw;
					delete[] log;
					send(client, answ.c_str(), 3, NULL);
					int count = 0;
					while (count < 3)
					{
						    char* pswSz = new char[5];
						    rcv = 0;
					        rcv = recv(client, pswSz, 5, NULL);
							UINT pswSize = atoi(pswSz);
							char* psw = new char[pswSize];
							rcv = 0;
							rcv = recv(client, psw, pswSize, NULL);
						    std::string check( psw, pswSize );
							if ( check != db.passw )
							{
								std::cout << db.passw << " \n";
								std::cout << check << "\n";
								std::string answer = "N";
									send(client, answer.c_str(), 3, NULL);
									delete[] pswSz;
									delete[] psw;
									++count;
							}
							else
							{
								std::string answer = "Y";
								std::cout << "ACCESS GRANTED\n";
									send(client, answer.c_str(), 3, NULL);
									delete[] pswSz;
									delete[] psw;
									clntVec.push_back(client);
									break;
							}
					}
					std::string sock = std::to_string(server);
					std::string sockSz = std::to_string(sock.size());
					send(client, sockSz.c_str(), 5, NULL);
					send(client, sock.c_str(), sock.size(), NULL);

					
				}
			}
		}
	}

}