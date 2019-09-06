#pragma once

#define FALLBACK_PORT "7780"
#define SOCKET_OKAY 0

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdio.h>
#include <string>
#include <exception>
#include <vector>
#include <memory>

#include "Client.h"

using ClientList = std::vector<std::unique_ptr<Client>>;
using ClientPtr = std::unique_ptr<Client>;

class ServerSocket
{
	public: 
		ServerSocket(const std::string port = FALLBACK_PORT);
		~ServerSocket();
		void Listen();
		void Stop();
	private:
		SOCKET mListenSocket = INVALID_SOCKET;
		bool mDoListen = false;
		const int mPort;

		FD_SET readDescriptors, exceptionDescriptors, writeDescriptors;
		SOCKET maxFD = INVALID_SOCKET;
		ClientList mConnectedClients;

		void ZeroClientSockets();
		void CheckForConnections();
		void AcceptNewClient();
		void TerminateClient(ClientPtr const &client);
		void CheckClients();
		int Select();
		int ReadData(ClientPtr const &client);

};

