#ifndef FULL_INSTRUCTION_HPP
#define FULL_INSTRUCTION_HPP

#include "addrmode.hpp"
#include "instruction.hpp"

#include <cstdint>

struct FULLINSTRUCTION {
	std::uint8_t op_code;
	INSTRUCTION instr;
	ADDRMODE addr_mode;
	std::uint8_t bytes;
	std::uint8_t cycles;
};

#endif
