#pragma once

#include <WS2tcpip.h>
#include <atomic>
#include <queue>
#include <mutex>
#include "ClientPacket.h"

const std::size_t kClientBufferSize = 2048;

class Client {
	public: 
		Client();
		~Client();

		SOCKET socket = -1;
		struct sockaddr_in endpoint = { 0 };
		socklen_t saddr_len = sizeof(endpoint);
		char recvBuffer[kClientBufferSize];
		int recvBufferUsed = 0;
		std::mutex mMsgQueueMutex;

		const int GetID() const;
		bool IsTerminated();
		void TerminateConnection();
		void ClearBuffers();
		void CheckExtractPacket();
		void AppendClientMessage(ClientPacket packet);


		bool operator==(const Client& compareTarget) const;

	private:
		static std::atomic_int __id;
		int ID;
		bool mIsTerminated = false;

		std::queue<ClientPacket> clientMessages;
};

