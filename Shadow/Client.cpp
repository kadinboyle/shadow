#include "Client.h"
#include "easylogging++.h"

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

bool Client::IsTerminated() {
	return mIsTerminated;
}

