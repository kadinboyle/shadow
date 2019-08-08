#pragma once

#include <WS2tcpip.h>
#include <atomic>

class Client {
	public: 
		Client();
		~Client();
		const int GetID() const;
		bool IsTerminated();
		bool operator==(const Client& compareTarget) const;
		SOCKET socket = -1;
		struct sockaddr_in endpoint = { 0 };
		socklen_t saddr_len = sizeof(endpoint);
		void TerminateConnection();

	private:
		static std::atomic_int __id;
		int ID;
		bool mIsTerminated = false;
};

