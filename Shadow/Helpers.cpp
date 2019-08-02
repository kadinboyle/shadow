#include "Helpers.h"
#include <WS2tcpip.h>

char timeBuffer[9];

std::string getTimeNow() {
	//std::time_t time_now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	//struct tm* time_info = localtime(&time_now);
	//strftime(timeBuffer, 9, "%H:%M:%S", time_info);
	//return std::string(timeBuffer);
	return "NOT IMPLEMENTED";
}

std::string inet4_ntop_str(sockaddr_in const* addrStruct) {
	char strBuf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addrStruct->sin_addr), strBuf, INET_ADDRSTRLEN);
	return std::string(strBuf);
}

std::string inet6_ntop_str(sockaddr_in6 const* addrStruct) {
	char strBuf[INET6_ADDRSTRLEN];
	return std::string(inet_ntop(AF_INET6, &addrStruct->sin6_addr, strBuf, INET6_ADDRSTRLEN));
}

std::string inet_ntop_str(sockaddr* addrStruct) {
	if (addrStruct->sa_family == AF_INET) {
		return inet4_ntop_str((sockaddr_in const*)addrStruct);
	}
	else if (addrStruct->sa_family == AF_INET6) {
		return inet6_ntop_str((sockaddr_in6 const*)addrStruct);
	}
	return "";
}