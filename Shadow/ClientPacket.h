#pragma once

#include <cstddef>

const std::size_t kMaxPacketSize = 2048;

class ClientPacket {

	public: 
		char packetBuffer[kMaxPacketSize];
		bool Parse();

	private:
		bool isValid = false;
		bool isParsed = false;

};

