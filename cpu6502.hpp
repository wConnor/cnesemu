#ifndef CPU6502_HPP
#define CPU6502_HPP

#include "fullinstruction.hpp"
#include "bus.hpp"

#include <spdlog/spdlog.h>

#include <cmath>
#include <bitset>
#include <string>
#include <stack>
#include <array>
#include <algorithm>
#include <memory>
#include <limits>

constexpr double CLOCK_SPEED_MHZ = 1.789773;
constexpr std::uint8_t STACK_SIZE = 255;
constexpr std::uint8_t INSTRUCTION_COUNT = 255;

class CPU6502
{
public:
	CPU6502();
	void fetch_fullinstruction(const std::uint8_t &op_code);
	void decode_instruction(const std::uint16_t &operand);
	void exec_instruction(const std::uint16_t &address);
	void clear();
	virtual ~CPU6502();

	void set_bus(const std::shared_ptr<Bus> &bus);

	std::uint8_t read(const std::uint16_t &addr);
	void write(const std::uint16_t &addr, const std::uint8_t &data);

	void nes_init_regs();

	void clock_cycle();

private:
	/* registers: accumulator, x, y, stack pointer, status register, program counter
	   status register: bit 5 can not be changed and is always set to 1.
	   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	   | N | V | - | B | D | I | Z | C | */
	std::uint8_t acc, x, y, sp, sr;
	std::uint16_t pc;

	std::stack<std::uint8_t> stack;

	FULLINSTRUCTION current_instruction;

	std::shared_ptr<Bus> bus;
	std::uint8_t cycles = 0;

	std::array<FULLINSTRUCTION, INSTRUCTION_COUNT> instr_matrix;
};

#endif
