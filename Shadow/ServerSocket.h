#pragma once

#define FALLBACK_PORT "7780"

#include <WinSock2.h>
#include <WS2tcpip.h>

#include <stdio.h>
#include <string>
#include <exception>
#include <vector>

#include "Client.h"

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

		FD_SET readDescriptors;
		SOCKET maxFD = INVALID_SOCKET;
		std::vector<Client> mConnectedClients;

		void ZeroClientSockets();
		void CheckForConnections();
		void AcceptNewClient();
		int Select();

};

