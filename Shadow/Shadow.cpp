// Shadow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "easylogging++.h"
#include <iostream>

#include "ServerSocket.h"
#include "Utilities.h"
#include <Windows.h>
#include "MDialog.h"

INITIALIZE_EASYLOGGINGPP


void configureLogger() {
	el::Configurations defaultConf;
	defaultConf.setToDefault();
	defaultConf.set(el::Level::Global, el::ConfigurationType::Format, "%datetime{%d/%M/%y %h:%m:%s}[%level]: %msg");
	defaultConf.set(el::Level::Global, el::ConfigurationType::ToFile, "false");
	el::Loggers::reconfigureLogger("default", defaultConf);
}

int main() {
	configureLogger();
	LOG(INFO) << "Initializing...";

	//MessageBox(nullptr, TEXT("The driver is sleeping!!"), TEXT("Message"), MB_OK);
	DoDebugDialog(nullptr, nullptr);
	LOG(WARNING) << "GET BACK ";

	//TODO: Make server periodically stop listening if no active tasks?
	ServerSocket server;
	server.Listen();
	server.Stop();
	std::cin.get();
}
