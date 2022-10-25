#ifndef RP2A03_HPP
#define RP2A03_HPP

#include <bitset>
#include <string>
#include <stack>
#include <array>
#include <cstdint>

constexpr std::uint16_t RAM_SIZE = 2048u;

class RP2A03
{
public:
	RP2A03();
	void exec_instruction(std::uint8_t &opcode);
	virtual ~RP2A03();

private:
	/* registers: accumulator, x index, y index, stack pointer, status register, program counter
	   status register: bit 5 can not be changed and is always set to 1. */
	std::uint8_t acc, ix, iy, sp, sr;
	std::uint16_t pc;

	std::stack<std::uint8_t> stack;
	std::array<std::uint8_t, RAM_SIZE> ram;

};

#endif
