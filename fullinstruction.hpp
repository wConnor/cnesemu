#ifndef FULL_INSTRUCTION_HPP
#define FULL_INSTRUCTION_HPP

#include <cstdint>
#include <string>

struct FULLINSTRUCTION {
	std::uint8_t opcode;
	std::string instr;
	std::string addr_mode;
	std::uint8_t bytes;
	std::uint8_t cycles;
};

#endif
