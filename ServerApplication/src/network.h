//
// Created by nicol on 01/10/2021.
//

#include <WS2tcpip.h>
#include <stdio.h>

#include "terminal.h"
#include "connection.h"
#include <string>
#include "TCPConnection.h"

#pragma comment(lib, "Ws2_32.lib")

//#define DEFAULT_BUFLEN 512

#ifndef RES_TP1_NETWORK_H
#define RES_TP1_NETWORK_H

class network {
public:
	network(int protocol, std::string ipAdress, int port);
	void InitListen(addrinfo* result);
	void Connect(addrinfo* result);
	void ListenUpdate(SOCKET socketToListen);
	void ListenClient(TCPConnection connection);
	SOCKET getListenSocket() { return listenSocket; };
private:
	SOCKET listenSocket = INVALID_SOCKET;
	fd_set socketList;

};


#endif //RES_TP1_NETWORK_H
