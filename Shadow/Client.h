#pragma once

#include <WS2tcpip.h>

class Client {
	public: 
		SOCKET socket = -1;
		struct sockaddr_in endpoint;
		socklen_t saddr_len = sizeof(endpoint);
};

