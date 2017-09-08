#include "Comm.h"
#include <string.h>
#include <sys/socket.h>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <fstream>
#include <random>
#include <netdb.h>
#include <netinet/in.h>

int main(){
	int error;
	Comms::comms x;
	const SOCKET serv=x.host("192.168.2.51", "5505");
	SOCKET client = 0;
	sockaddr y;
	while (client == 0)
		client = x.Listen(serv);

	char buffer[640*480];
	char ack[2];
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(0, 255);
	std::uniform_int_distribution<> go(0, 1);
	int j = 0;
	int i = 0;
	int kleur = 0;
	while (1) {
		kleur++;
		if (kleur > 255)kleur = 0;
		j = 0;  i = 0;
		//while (j < 640 * 480) {
		memset(buffer, kleur, 640 * 480);
		ack[0] = 0;
		error = x.rcv(client, ack, 2);
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		if (error == 10060)
			continue;
		if (error != 0)
			return -1;
		for (i = 0; i < 900&&j<640*480; i++) {
			buffer[i] = dis(gen),j++;
		}
		send(client, buffer, 640 * 480, 0);
	}
	return 0;
}
