#pragma once

//Command title occupys first byte of a packet, starting at 0x10 - 0xFF

enum PacketType {
	CMD_SET_BOT_NAME = 0x10,
	CMD_RESTART_BOT = 0x11,
	CMD_TAKE_SCREENSHOT = 0x12,
	CMD_SHOW_MSG_BOX = 0x13
};

//constexpr char SetBotName = 0x10;
//constexpr char RestartBot = 0x11;
//constexpr char TakeScreenshot = 0x12;
//constexpr char MessageBox = 0x13;