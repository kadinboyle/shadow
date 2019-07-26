#pragma once

#define FALLBACK_PORT "7780"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

class ServerSocket
{
	public: 
		ServerSocket(const std::string port = FALLBACK_PORT);
		~ServerSocket();
	private:
		SOCKET listenSocket = INVALID_SOCKET;


};

