
#define COMM_EXPORTS
#include "Comm.h"
#include <thread>
#include <string.h>	//string
#include <iostream>  //cout
#include <sys/socket.h>
#include <cstring>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
int WSAGetLastError(){
	return 666;
}

namespace Comms {

	const SOCKET comms::setup(const char* IP, const char* PORT) {
		int error = 0;
		SOCKET client = INVALID_SOCKET;
		struct addrinfo *result = NULL, hint;
		memset(&hint,0, sizeof(hint));  //put everything to 0
		hint.ai_family = AF_INET; //ipv4
		hint.ai_socktype = SOCK_STREAM; //stream aka keep connection alive
		hint.ai_protocol = IPPROTO_TCP; //tcp		
		error = getaddrinfo(IP, PORT, &hint, &result);
		if (error != 0) {
			std::cout << "error in getting addr: " << error << std::endl;
			return -1;
		}
		client = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (client == INVALID_SOCKET) {
			std::cout << "error in client socket: " << WSAGetLastError() << std::endl;
			return -1;
		}
		char* sa = "10";
		error = setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, sa, sizeof(sa));
		if (error == SOCKET_ERROR) {
			error = WSAGetLastError();
			std::cout << "unable to alter socket options: " << error << std::endl;
			close(client);
			return -1;
		}
		error = connect(client, result->ai_addr, (int)result->ai_addrlen);
		if (error == SOCKET_ERROR) {
			error = WSAGetLastError();
			std::cout << "error in connecting to server: " << error << std::endl;
			close(client);
			//try other stuff For now just quit		
			return -1;
		}
		struct sockaddr_in sock; socklen_t len = sizeof(sock);
		error = getsockname(client, (struct sockaddr*)&sock, &len);
		if (error == SOCKET_ERROR) {
			error = WSAGetLastError();
			std::cout << "error in getting port: " << error << std::endl;
			close(client);
			//try other stuff For now just quit		
			return -1;
		}
		std::cout << "running client on socket: " << (uint16_t)client << " -Port: " << ntohs(sock.sin_port) << std::endl;
		return client;
	}

	const SOCKET comms::host(const char* ip, const char* port) {
		int error = 0;
		SOCKET server = INVALID_SOCKET;
		struct addrinfo *result = NULL, hint;
		memset(&hint,0, sizeof(hint));  //put everything to 0
		hint.ai_family = AF_INET; //ipv4
		hint.ai_socktype = SOCK_STREAM; //stream aka keep connection alive
		hint.ai_protocol = IPPROTO_TCP; //tcp
		hint.ai_flags = AI_PASSIVE;  //use returned socket address in bind	
		error = getaddrinfo(ip, port, &hint, &result);
		if (error != 0) {
			std::cout << "error in server addr: " << error << std::endl;
			return 0;
		}

		server = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (server == INVALID_SOCKET) {
			std::cout << "error in server socket: " << WSAGetLastError() << std::endl;
			return 0;
		}

		error = bind(server, result->ai_addr, (int)result->ai_addrlen);

		if (error == SOCKET_ERROR) {
			std::cout << "error in binding server socket: " << WSAGetLastError() << std::endl;
			return 0;
		}
		sockaddr y;
		memset(&y, 0,sizeof(y));
		y.sa_family = result->ai_addr->sa_family;
		std::cout << "running server on socket: " << (uint16_t)server << std::endl;
		std::cout << ntohs(((sockaddr_in*)(result->ai_addr))->sin_port) << std::endl;
		return server;
	}

	const SOCKET comms::Listen(const SOCKET server) {
		int error = 0;
		sockaddr y;
		error = listen(server, 8);
		if (error == SOCKET_ERROR) {
			std::cout << "error in listening: " << WSAGetLastError() << std::endl;
			return 0;
		}
		socklen_t size = sizeof(y);
		error = accept(server, &y, &size);
		if (error == -1) {
			std::cout << "error in accepting connection: " << WSAGetLastError() << std::endl;
			return 0;
		}
		if (error > 0) {
			int place;
			char* tv = "10";
			place = setsockopt(error, SOL_SOCKET, SO_RCVTIMEO, tv, sizeof(tv));
			std::cout << "added client on socket: " << (uint16_t)error << std::endl;
			if (error == 0) {
				std::cout << "error in timeout setting: " << WSAGetLastError() << std::endl;
				return 0;
			}
			else {
				return error;
			}
		}
		return 0;
	}

	const int comms::snd(const SOCKET x, const char* message, const int len) {
		bool prep = 1;
		int loops = 0;
		int iter = 0;
		char* b = new char[len];
		memset(b,0, len);
		int status;
		while (prep) {
			status = strlen(message) - iter+1;
			if (status > len) {				
				std::memcpy(b, &message[iter],len-1);
				iter += len - 1;								
				b[len - 1] = 0;
				status = 1;
				status = send(x, b, strlen(b)+1, 0);
				if (status == SOCKET_ERROR) {
					status = WSAGetLastError();
					std::cout << "error in reading of data: " << status << std::endl;
					delete[] b;
					return status;
				}
				if (status == 0) {
					std::cout << "problem" << std::endl;
					delete[] b;
					return 1;
				}
			}
			else {
				std::memcpy(&b[0],message,len);
				b[len - 1] = 0;
				prep = 0;
			}
		}
		status = 1;
		status = send(x, b, strlen(b)+1, 0);
		if (status == SOCKET_ERROR) {
			status = WSAGetLastError();
			std::cout << "error in reading of data: " << status << std::endl;
			return status;
		}
		if (status == 0) {
			std::cout << "problem" << std::endl;
			return 1;
		}
		delete []b;
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		
		return 0;
	}

	const char* comms::dnslookup(const char* url) {
	/*	int error = 0;
		SOCKET client = INVALID_SOCKET;
		struct addrinfo *result = NULL, hint;
		memset(&hint,0, sizeof(hint));  //put everything to 0
		hint.ai_family = AF_INET; //ipv4
		hint.ai_socktype = SOCK_STREAM; //stream aka keep connection alive
		hint.ai_protocol = IPPROTO_TCP; //tcp				
		error = getaddrinfo(url, "6667", &hint, &result);
		int y = 0;
		struct hostent *x;
		x=gethostbyname(url);		
		std::cout << x->h_addrtype << std::endl;
		std::cout << x->h_addr_list << std::endl;
		std::cout << x->h_aliases << std::endl;
		std::cout << x->h_length << std::endl;
		std::cout << x->h_name << std::endl;
		y = 0;*/
		return nullptr;
	}

	const int comms::rcv(const SOCKET x, char* message, const int len) {
		int status = 1;				
		status = recv(x, message, len, 0);
		if (status == SOCKET_ERROR) {
			status = WSAGetLastError();
			if (status == 10060) {
		//		std::cout << "no data on socket: " << (uint16_t)x << std::endl;
				return status;
			}
			if (status == 10054) return status;
			std::cout << "error in reading of data: " << status << std::endl;
			return status;
		}
		if (status == 0) {
			std::cout << "skipped recv" << std::endl;
			return 10054;
		}
		return 0;
	}
}
