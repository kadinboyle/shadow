#pragma once

#include <cstddef>
#include <cstdint>

const char kPacketDelimeter = 0x1B;

namespace Util {

	size_t StuffBytes(const uint8_t *input, size_t length, uint8_t *output);
	size_t UnstuffBytes(const uint8_t *input, size_t length, uint8_t *output);

}