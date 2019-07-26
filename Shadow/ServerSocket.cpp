#include "ServerSocket.h"
#include "easylogging++.h"

ServerSocket::ServerSocket(const std::string port) 
{
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int res = getaddrinfo(NULL, port.c_str(), &hints, &result);
	if (res != 0) {
		LOG(ERROR) << "getaddrinfo failed: " << gai_strerror(res);
		//std::cout << 
	}
}