#include "ClientPacket.h"
#include "CCBotCommands.h"
#include "easylogging++.h"


using namespace std;

//echo -ne "\x13blah\033" | nc 127.0.0.1 7780  

bool ClientPacket::Parse() {
	bool parseOk = false;

	const char command = this->packetBuffer[0];
	switch (command){
		case PacketType::CMD_SET_BOT_NAME:
			LOG(INFO) << "COMMAND: SetBotName";
			break;
		case PacketType::CMD_RESTART_BOT:
			LOG(INFO) << "COMMAND: RestartBot";
			break;
		case PacketType::CMD_TAKE_SCREENSHOT:
			LOG(INFO) << "COMMAND: TakeScreenshot";
			break;
		case PacketType::CMD_SHOW_MSG_BOX:
			LOG(INFO) << "COMMAND: ShowMessageBox";
			break;

		default:
			break;
	}

	if (parseOk)
		isValid = true;
	
	isParsed = true;
	return parseOk;
}