#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <string>

struct INSTRUCTION {
	std::uint8_t opcode;
	std::string instr;
	std::string addr_mode;
	std::uint8_t bytes;
	std::uint8_t cycles;
};

#endif
