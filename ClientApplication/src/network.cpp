#include "network.h"
#include <iostream>
#include <thread>

#include "TCPConnection.h"
#include "UDPConnection.h"

network::network(int protocol, std::string ipAdress, int port)
{

	WSADATA wsaData;
	int iResult;
	struct addrinfo* result = NULL;
	struct addrinfo hints;

	//Initialise Winsock
	printf("\nInitialising Winsock...");
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
	}

	printf("Initialised.\n");
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_PASSIVE;

	if (protocol == 0) {
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
	}
	else if (protocol == 1) {
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
	}

	const char* ipAdressAsPCSTR = ipAdress.c_str();
	std::string portAsString = std::to_string(port);
	char const* portAsPCSTR = portAsString.c_str();

	// Resolve the server address and port
	iResult = getaddrinfo(ipAdressAsPCSTR, portAsPCSTR, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
	}
	Connect(result);
}

//Cr��e Connection pour le client + cr��e socket de connection au serveur
void network::Connect(addrinfo* result)
{
	// Create a SOCKET for connecting to server
	SOCKET connectSocket = socket(result->ai_family, result->ai_socktype,
		result->ai_protocol);
	if (connectSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		getchar();
	}

	// Connect to server.
	int iResult = connect(connectSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
	}

	if (connectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		getchar();
	}

	puts("Connected to server.\n");

	if (result->ai_protocol == IPPROTO_UDP)
	{
		//UDP FOR LATER TODO
		/*UDPConnection connection = UDPConnection(listenSocket, connectSocket);
		std::thread listenThread(&network::ListenServer, this, connection);
		if (listenThread.joinable())
			listenThread.join();

		std::thread listenInputThread(&terminal::ListenInput, terminal(), connection);
		if (listenInputThread.joinable())
			listenInputThread.join();
		*/
	}
	else if (result->ai_protocol == IPPROTO_TCP)
	{
		TCPConnection connection = TCPConnection(listenSocket, connectSocket);
		std::thread listenThread(&network::ListenServer, this, connection);
		std::thread listenInputThread(&terminal::ListenInputTCP, terminal(), connection);

		if (listenThread.joinable())
			listenThread.join();
		if (listenInputThread.joinable())
			listenInputThread.join();
	}

	freeaddrinfo(result);
}

void network::ListenServer(TCPConnection connection)
{

	while (true)
	{
		char* message = connection.Receive();
		//leave if fail TODO
	}

}

