#include "Client.h"
#include "ClientPacket.h"
#include "Utilities.h"
#include "easylogging++.h"
#include <string>
#include <algorithm>
#include <iterator>

const int Client::GetID() const {
	return ID;
}

Client::Client() {
	static int __id = 1;
	ID = __id++;
	//LOG(DEBUG) << "Client " << ID << " Constructed";
}

Client::~Client() {
	TerminateConnection();
	//LOG(DEBUG) << "Client " << ID << " destructed";
}

bool Client::operator==(const Client& compareTarget) const {
	return this->ID == compareTarget.GetID();
}

void Client::TerminateConnection() {
	if (mIsTerminated == true)
		return;

	mIsTerminated = true;
	closesocket(socket);
}

void Client::ClearBuffers() {
	std::fill(std::begin(recvBuffer), std::end(recvBuffer), 0);
	recvBufferUsed = 0;
}

void Client::AppendClientMessage(ClientPacket packet) {
	if (!packet.Parse()) {
		return;
	}

	std::lock_guard<std::mutex> lock(mMsgQueueMutex);
	clientMessages.push(packet); //will copy
}

//TODO: This probably shouldn't be recursive
void Client::CheckExtractPacket() {
	std::string_view sv(recvBuffer, std::size(recvBuffer));

	if (recvBufferUsed < 1)
		return;

	if (std::size_t n = sv.find(kPacketDelimeter); n != sv.npos){
		LOG(INFO) << "Packet delimter found at offset: " << n << " bytes";
		if (n == 0) {//need to handle this case properly. Consume this?
			LOG(WARNING) << "DELIM AT 0...........";
			recvBufferUsed--; //we are removing a byte
			if(recvBufferUsed != 0)
				memmove(recvBuffer, recvBuffer + 1, recvBufferUsed);
			memset(recvBuffer + recvBufferUsed, 0, kClientBufferSize - recvBufferUsed);
			return CheckExtractPacket();
		}
			

		const unsigned int remainingBytes = (recvBufferUsed - n) - 1; //-1 for discarding delim

		if (n > kMaxPacketSize) {
			//signal error
			LOG(WARNING) << "Packet size exceeded";
		}
		else {
			ClientPacket packet;
			memcpy(packet.packetBuffer, recvBuffer, n);
			memset(packet.packetBuffer + n, '\0', 1);
			LOG(INFO) << "PACKET CONTENTS DUMP: [" << packet.packetBuffer << "]";
			packet.Parse();
			//AppendClientMessage(packet);
		}

		LOG(WARNING) << clientMessages.size() << " QUEUE SIZE";

		//TODO: CHeck here if nremainingbytes > 0? Or is that covered by recvBufferUsed assignment below

		LOG(INFO) << "Resetting with " << remainingBytes << " bytes";
		memmove(recvBuffer, (recvBuffer + n) + 1, remainingBytes);
		memset(recvBuffer + remainingBytes, 0, kClientBufferSize - remainingBytes); //check this for ending esc
		recvBufferUsed = remainingBytes;
		
		//LOG(INFO) << "BUFF DUMP: [" << recvBuffer << "] REMAINING BYTES:" << remainingBytes;
		if(remainingBytes > 0)
			return CheckExtractPacket();
	}
}

bool Client::IsTerminated() {
	return mIsTerminated;
}

