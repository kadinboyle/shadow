#include "Client.h"
#include "easylogging++.h"

const int Client::GetID() const {
	return ID;
}

Client::Client() {
	static int __id = 1;
	ID = __id++;
	LOG(INFO) << "Client " << ID << " Constructed";
}

Client::~Client() {
	TerminateConnection();
	LOG(INFO) << "Client " << ID << " destructed";
}

bool Client::operator==(const Client& compareTarget) const {
	return this->ID == compareTarget.GetID();
}

void Client::TerminateConnection() {
	if (mIsTerminated == true)
		return;

	mIsTerminated = true;
	closesocket(socket);
	LOG(INFO) << "Client " << ID << " socket terminated";
}

bool Client::IsTerminated() {
	return mIsTerminated;
}

