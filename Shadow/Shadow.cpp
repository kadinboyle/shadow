// Shadow.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "easylogging++.h"
#include <iostream>

INITIALIZE_EASYLOGGINGPP



void configureLogger() {
	el::Configurations defaultConf;
	defaultConf.setToDefault();
	defaultConf.set(el::Level::Global, el::ConfigurationType::Format, "%datetime{%d/%M/%y %h:%m:%s}[%level]: %msg");
	defaultConf.set(el::Level::Global, el::ConfigurationType::ToFile, "false");
	el::Loggers::reconfigureLogger("default", defaultConf);
}

int main()
{
	configureLogger();
    std::cout << "Hello World!\n";
	LOG(INFO) << "GIDDAY";
}