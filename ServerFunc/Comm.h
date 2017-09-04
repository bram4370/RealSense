#pragma once

//#define COMM_API

#include <WinSock2.h>


#ifdef COMM_EXPORTS
#define COMM_API __declspec(dllexport)
#else
#define COMM_API __declspec(dllimport)
#endif // COMM_EXPORT

namespace Comms
{
	class comms {
	public:
		//listen on port and accept + return incoming transmissions
		static COMM_API const SOCKET Listen(const SOCKET server);
		//set-up server with given parameters
		static COMM_API const SOCKET host(const char* ip, const char* port);
		//get the ip address of url
		static COMM_API const char* dnslookup(const char* url);
		//esthablishes a connection with given parameters
		static COMM_API const SOCKET setup(const char* ip, const char* port);
//Send message over socket returns 0 on succes
		static COMM_API const int snd(const SOCKET x, const char* message,const int len);
//Retrieve messages over socket returns 0 on succes 10060 when no data has arrived
		static COMM_API const int rcv(const SOCKET x, char* message, const int len);
	};
}

