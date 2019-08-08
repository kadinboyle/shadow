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