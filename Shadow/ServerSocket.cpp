#include "ServerSocket.h"
#include "easylogging++.h"
#include "Helpers.h"


ServerSocket::ServerSocket(const std::string port) : mPort(std::stoi(port)) {
	WSADATA wsaData;
	int res;
	int optEnable = 1;

	res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != NO_ERROR) {
		LOG(ERROR) << "WSAStartup() failed";
		throw std::exception("WSAStartup() error");
	}

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;

	res = getaddrinfo(NULL, port.c_str(), &hints, &result);
	if (res != 0) {
		LOG(ERROR) << "getaddrinfo() failed: " << GetWSAErrorString();
		WSACleanup();
		throw std::exception("Failed getaddrinfo() on server socket creation");
	}

	mListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (mListenSocket == INVALID_SOCKET) {
		LOG(ERROR) << "socket() failed: " << GetWSAErrorString();
		WSACleanup();
		freeaddrinfo(result);
		throw std::exception("socket() failed");
	}

	res = bind(mListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR) {
		LOG(ERROR) << "bind() failed: " << GetWSAErrorString();
		freeaddrinfo(result);
		this->Stop();
		throw std::exception("bind() failed");
	}

	freeaddrinfo(result);

	res = setsockopt(mListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optEnable, sizeof(int));
	if (res == SOCKET_ERROR) 
		LOG(ERROR) << "setsockopt() SO_REUSEADDR failed: " << GetWSAErrorString();

	u_long enable = 1;
	res = ioctlsocket(mListenSocket, FIONBIO, &enable);
	if (res == SOCKET_ERROR)
		LOG(ERROR) << "ioctlsocket() non blocking on server socket failed: " << GetWSAErrorString();
	
}
//https://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5a.html
ServerSocket::~ServerSocket() {

}

void ServerSocket::Listen(){
	if (listen(mListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		LOG(ERROR) << "listen() server socket failed: " << GetWSAErrorString();
		this->Stop();
	}

	mDoListen = true;
	LOG(INFO) << "Server listening on " << mPort;
	int activity = 0;
	SOCKET maxSockDesc = INVALID_SOCKET;

	while (mDoListen == true) {
		//add valid client sock descs to set for checking
		ZeroClientSockets();

		//wait for activity
		activity = Select();
		if (activity < 0) {
			LOG(ERROR) << "select() error: " << GetWSAErrorString();
			mDoListen = false;
		}

		//general IO
		this->CheckClients();
			
		//check for new connections
		this->CheckForConnections();




	 }

}

void ServerSocket::Stop(){
	closesocket(mListenSocket);
	WSACleanup();
}

void ServerSocket::ZeroClientSockets() {
	FD_ZERO(&readDescriptors);
	FD_ZERO(&writeDescriptors);
	FD_ZERO(&exceptionDescriptors);

	//add listen socket
	FD_SET(mListenSocket, &readDescriptors);
	FD_SET(mListenSocket, &exceptionDescriptors);

	for (auto client = mConnectedClients.begin(); client != mConnectedClients.end(); ++client) {
		if ((*client)->socket != INVALID_SOCKET) {

			FD_SET((*client)->socket, &readDescriptors); //check for buffer space?
			FD_SET((*client)->socket, &exceptionDescriptors);
		}
		else {
			LOG(ERROR) << "Removing client";
			//call TerminateClient instead?
			client = mConnectedClients.erase(client);
			if (client == mConnectedClients.end())
				break;
		}
	}
}

int ServerSocket::Select(){
	return select(0, &readDescriptors, &writeDescriptors, &exceptionDescriptors, NULL);
}

int ServerSocket::ReadData(ClientPtr const &client) {
	int err;

	if (client->recvBufferUsed >= kClientBufferSize) {
		LOG(ERROR) << "Receive buffer is full for Client " << client->GetID();
		
		return SOCKET_ERROR;
	}

	while (client->recvBufferUsed < kClientBufferSize) {
		int bytesRead = recv(client->socket, client->recvBuffer + client->recvBufferUsed, kClientBufferSize - client->recvBufferUsed, 0);
		
		if (bytesRead == 0) {
			return WSAECONNRESET; //is this case ever hit in nonblocking socket? Maybe remove
		}
		else if (bytesRead == SOCKET_ERROR) {
			err = WSAGetLastError();

			if (err == WSAEWOULDBLOCK) {
				return SOCKET_OKAY; //fine, as using nonblocking socket
			}
			
			return err;
		}

		client->recvBufferUsed += bytesRead;

	} 
	//DONT FORGET TO CLEAR VARIABLES ONCE BUFFER DATA EXTRACTED

	return SOCKET_OKAY;
}

void ServerSocket::CheckClients() {

	for (auto &client : mConnectedClients) {
		int socketStatus = SOCKET_OKAY;

		if (FD_ISSET(client->socket, &exceptionDescriptors)) {
			LOG(WARNING) << "Client " << client->socket << " exception descriptor set";
			socketStatus = SOCKET_ERROR;
			FD_CLR(client->socket, &exceptionDescriptors);
		}
		else {
			if (FD_ISSET(client->socket, &readDescriptors)) {
				LOG(DEBUG) << "Client " << client->GetID() << " has data!";
				socketStatus = ReadData(client);
			}
			/*if (FD_ISSET(client->socket, &writeDescriptors)) {

			}*/
		}


		if (socketStatus != SOCKET_OKAY) {
			if (socketStatus == WSAECONNRESET)
				LOG(ERROR) << "Client Terminated connection";
			else
				LOG(ERROR) << "Client " << client->GetID() << ": " << GetWSAErrorString();
			client->socket = INVALID_SOCKET; //mark as invalid, will be erased by zero/check fds at start of loop
			//TerminateClient((*client));
		}
		else {
			//Check extract packet from buffer
			client->CheckExtractPacket();
			LOG(INFO) << "Done checking extract packet";
		}
	}
}


void ServerSocket::CheckForConnections(){
	if (FD_ISSET(mListenSocket, &readDescriptors)) {
		AcceptNewClient();
	}
}

//TODO: Check this okay with vector removal
void ServerSocket::TerminateClient(ClientPtr const &client) {
	if (client->IsTerminated())
		return;
	LOG(INFO) << "Terminating Client " << client->GetID();

	//FD_CLR(client->socket, &readDescriptors);
	//FD_CLR(client->socket, &

	ClientList::iterator it = std::find(mConnectedClients.begin(), mConnectedClients.end(), client);
	if (it != mConnectedClients.end())
		mConnectedClients.erase(it); //erasing will trigger clients Destructor and Terminate method which will closesocket
	else
		LOG(WARNING) << "Client not found in list";
}


void ServerSocket::AcceptNewClient(){
	ClientPtr client(new Client);
	u_long nonBlockingEnable = 1;

	LOG(INFO) << "Client [" << client->GetID() << "] accepted";
	SOCKET newConnection = accept(mListenSocket, (struct sockaddr*) & client->endpoint, &client->saddr_len);
	if (newConnection == INVALID_SOCKET) {
		LOG(ERROR) << "Failed to accept client connection";
		closesocket(newConnection);
		return;
	}

	int res = ioctlsocket(newConnection, FIONBIO, &nonBlockingEnable);
	if (res != NO_ERROR) {
		LOG(ERROR) << "ioctlsocket() set non blocking on incoming client failed: " << GetWSAErrorString();
		return;
	}

	client->socket = newConnection;

	char port[6] = { 0 };
	int nw = snprintf(port, 6, "%u", ntohs(client->endpoint.sin_port));
	LOG(INFO) << "Connection accepted from " << inet4_ntop_str(&client->endpoint) << ":" << port;

	mConnectedClients.push_back(std::move(client));
}
