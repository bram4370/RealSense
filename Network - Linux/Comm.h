#pragma once

//#define COMM_API
#include <sys/types.h>
#include <sys/socket.h>


typedef int SOCKET ;
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

namespace Comms
{
	class comms {
public:
		//listen on port and accept + return incoming transmissions
		const SOCKET Listen(const SOCKET server);
		//set-up server with given parameters
		const SOCKET host(const char* ip, const char* port);
		//get the ip address of url
		const char* dnslookup(const char* url);
		//esthablishes a connection with given parameters
		const SOCKET setup(const char* ip, const char* port);
//Send message over socket returns 0 on succes
		const int snd(const SOCKET x, const char* message,const int len);
//Retrieve messages over socket returns 0 on succes 10060 when no data has arrived
		const int rcv(const SOCKET x, char* message, const int len);
	};
}

