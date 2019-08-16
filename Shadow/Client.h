#pragma once

#include <WS2tcpip.h>
#include <atomic>

const int kClientBufferSize = 2048;

class Client {
	public: 
		Client();
		~Client();

		SOCKET socket = -1;
		struct sockaddr_in endpoint = { 0 };
		socklen_t saddr_len = sizeof(endpoint);
		char recvBuffer[kClientBufferSize];
		int recvBufferUsed = 0;

		const int GetID() const;
		bool IsTerminated();
		void TerminateConnection();



		bool operator==(const Client& compareTarget) const;

	private:
		static std::atomic_int __id;
		int ID;
		bool mIsTerminated = false;
};

