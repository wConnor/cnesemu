#ifndef CPU6502_HPP
#define CPU6502_HPP

#include "fullinstruction.hpp"
#include "bus.hpp"

#include <spdlog/spdlog.h>

#include <map>
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
	std::uint8_t fetch();

	std::uint8_t read(const std::uint16_t &addr);
	void write(const std::uint16_t &addr, const std::uint8_t &data);

	void clock_cycle();

private:
	/* registers: accumulator, x, y, stack pointer, status register, program counter
	   status register: bit 5 can not be changed and is always set to 1.
	   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	   | N | V | - | B | D | I | Z | C | */
	std::uint8_t acc = 0x00;
	std::uint8_t x = 0x00;
	std::uint8_t y = 0x00;
	std::uint8_t sp = 0x00;
	std::uint8_t sr = 0b00100000;
	std::uint16_t pc = 0x0000;

	std::stack<std::uint8_t> stack;

	FULLINSTRUCTION current_instruction;

	std::shared_ptr<Bus> bus = nullptr;
	std::uint8_t opcode = 0x00;
	std::uint8_t fetched = 0x00;
	std::uint16_t addr_abs = 0x0000;
	std::uint16_t addr_rel = 0x0000;
	std::uint8_t cycles_left = 0;

	std::unordered_map<std::string, std::function<std::uint8_t()>> addr_mode_map = {
		{ "ABS", std::bind(&CPU6502::ABS, this) }, // absolute
		{ "ABX", std::bind(&CPU6502::ABX, this) }, // absolute, offset by x reg
		{ "ABY", std::bind(&CPU6502::ABY, this) }, // absolute, offset by y reg
		{ "ACC", std::bind(&CPU6502::ACC, this) }, // accumulator
		{ "IND", std::bind(&CPU6502::IND, this) }, // indirect
		{ "IZX", std::bind(&CPU6502::IZX, this) }, // indirect, offset by x reg
		{ "IZY", std::bind(&CPU6502::IZY, this) }, // indirect, offset by y reg
		{ "IMP", std::bind(&CPU6502::IMP, this) }, // implied
		{ "IMM", std::bind(&CPU6502::IMM, this) }, // immediate
		{ "ZPG", std::bind(&CPU6502::ZPG, this) }, // zero page
		{ "ZPX", std::bind(&CPU6502::ZPX, this) }, // zero page, offset by x reg
		{ "ZPY", std::bind(&CPU6502::ZPY, this) }, // zero page, offset by y reg
		{ "REL", std::bind(&CPU6502::REL, this) }, // relative

		{ "ILL", std::bind(&CPU6502::ILL, this) }, // illegal addressing mode
	};
	std::unordered_map<std::string, std::function<std::uint8_t()>> instr_map = {
		{ "ADC", std::bind(&CPU6502::ADC, this) }, // add with carry
		{ "AND", std::bind(&CPU6502::AND, this) }, // AND with accumulator
		{ "ASL", std::bind(&CPU6502::ASL, this) }, // arithmetic shift left
		{ "BCC", std::bind(&CPU6502::BCC, this) }, // branch on carry clear
		{ "BCS", std::bind(&CPU6502::BCS, this) }, // branch on carry set
		{ "BEQ", std::bind(&CPU6502::BEQ, this) }, // branch on equal (zero set)
		{ "BIT", std::bind(&CPU6502::BIT, this) }, // bit test
		{ "BMI", std::bind(&CPU6502::BMI, this) }, // branch on minus (negative set)
		{ "BNE", std::bind(&CPU6502::BNE, this) }, // branch on not equal (zero clear)
		{ "BPL", std::bind(&CPU6502::BPL, this) }, // branch on plus (negative clear)
		{ "BRK", std::bind(&CPU6502::BRK, this) }, // break / interrupt
		{ "BVC", std::bind(&CPU6502::BVC, this) }, // branch on overflow clear
		{ "BVS", std::bind(&CPU6502::BVS, this) }, // branch on overflow set
		{ "CLC", std::bind(&CPU6502::CLC, this) }, // clear carry
		{ "CLD", std::bind(&CPU6502::CLD, this) }, // clear decimal
		{ "CLI", std::bind(&CPU6502::CLI, this) }, // clear interrupt disable
		{ "CLV", std::bind(&CPU6502::CLV, this) }, // clear overflow
		{ "CMP", std::bind(&CPU6502::CMP, this) }, // compare with accumulator
		{ "CPX", std::bind(&CPU6502::CPX, this) }, // compare with x reg
		{ "CPY", std::bind(&CPU6502::CPY, this) }, // compare with y reg
		{ "DEC", std::bind(&CPU6502::DEC, this) }, // decrement
		{ "DEX", std::bind(&CPU6502::DEX, this) }, // decrement x reg
		{ "DEY", std::bind(&CPU6502::DEY, this) }, // decrement y reg
		{ "EOR", std::bind(&CPU6502::EOR, this) }, // XOR with accumulator
		{ "INC", std::bind(&CPU6502::INC, this) }, // increment
		{ "INX", std::bind(&CPU6502::INX, this) }, // increment x reg
		{ "INY", std::bind(&CPU6502::INY, this) }, // increment y reg
		{ "JMP", std::bind(&CPU6502::JMP, this) }, // jump
		{ "JSR", std::bind(&CPU6502::JSR, this) }, // jump subroutine
		{ "LDA", std::bind(&CPU6502::LDA, this) }, // load accumulator
		{ "LDX", std::bind(&CPU6502::LDX, this) }, // load x reg
		{ "LDY", std::bind(&CPU6502::LDY, this) }, // load y reg
		{ "LSR", std::bind(&CPU6502::LSR, this) }, // logical shift right
		{ "NOP", std::bind(&CPU6502::NOP, this) }, // no operation
		{ "ORA", std::bind(&CPU6502::ORA, this) }, // OR with accumulator
		{ "PHA", std::bind(&CPU6502::PHA, this) }, // push accumulator
		{ "PHP", std::bind(&CPU6502::PHP, this) }, // push status register (SR)
		{ "PLA", std::bind(&CPU6502::PLA, this) }, // push pull accumulator
		{ "PLP", std::bind(&CPU6502::PLP, this) }, // pull status register (SR)
		{ "ROL", std::bind(&CPU6502::ROL, this) }, // rotate left
		{ "ROR", std::bind(&CPU6502::ROR, this) }, // rotate right
		{ "RTI", std::bind(&CPU6502::RTI, this) }, // return from interrupt
		{ "RTS", std::bind(&CPU6502::RTS, this) }, // return from subroutine
		{ "SBC", std::bind(&CPU6502::SBC, this) }, // subtract with carry
		{ "SEC", std::bind(&CPU6502::SEC, this) }, // set carry
		{ "SED", std::bind(&CPU6502::SED, this) }, // set decimal
		{ "SEI", std::bind(&CPU6502::SEI, this) }, // set interrupt disable
		{ "STA", std::bind(&CPU6502::STA, this) }, // store accumulator
		{ "STX", std::bind(&CPU6502::STX, this) }, // store x reg
		{ "STY", std::bind(&CPU6502::STY, this) }, // store y reg
		{ "TAX", std::bind(&CPU6502::TAX, this) }, // transfer accumulator to x reg
		{ "TAY", std::bind(&CPU6502::TAY, this) }, // transfer accumulator to y reg
		{ "TSX", std::bind(&CPU6502::TSX, this) }, // transfer SP to x reg
		{ "TXA", std::bind(&CPU6502::TXA, this) }, // transfer x reg to accumulator
		{ "TXS", std::bind(&CPU6502::TXS, this) }, // transfer x reg to stack pointer
		{ "TYA", std::bind(&CPU6502::TYA, this) }, // transfer y reg to accumulator

		{ "ILL", std::bind(&CPU6502::ILL, this) }, // illegal operation
	};
	std::array<FULLINSTRUCTION, INSTRUCTION_COUNT> instr_matrix = {
		FULLINSTRUCTION
		{ 0x00, "BRK", "IMP", 1, 7 },
		{ 0x01, "ORA", "INX", 2, 6 },
		{ 0x02, "ILL", "ILL", 0, 0 },
		{ 0x03, "ILL", "ILL", 0, 0 },
		{ 0x04, "ILL", "ILL", 0, 0 },
		{ 0x05, "ORA", "ZPG", 2, 3 },
		{ 0x06, "ASL", "ZPG", 2, 5 },
		{ 0x07, "ILL", "ILL", 0, 0 },
		{ 0x08, "PHP", "IMP", 1, 3 },
		{ 0x09, "ORA", "IMM", 2, 2 },
		{ 0x0A, "ASL", "ACC", 0, 2 },
		{ 0x0B, "ILL", "ILL", 0, 0 },
		{ 0x0C, "ILL", "ILL", 0, 0 },
		{ 0x0D, "ORA", "ABS", 3, 4 },
		{ 0x0E, "ASL", "ABS", 3, 6 },
		{ 0x0F, "ILL", "ILL", 0, 0 },
		{ 0x10, "BPL", "REL", 2, 2 },
		{ 0x11, "ORA", "INY", 2, 5 },
		{ 0x12, "ILL", "ILL", 0, 0 },
		{ 0x13, "ILL", "ILL", 0, 0 },
		{ 0x14, "ILL", "ILL", 0, 0 },
		{ 0x15, "ORA", "ZPX", 2, 4 },
		{ 0x16, "ASL", "ZPX", 2, 6 },
		{ 0x17, "ILL", "ILL", 0, 0 },
		{ 0x18, "CLC", "IMP", 1, 2 },
		{ 0x19, "ORA", "ABY", 3, 4 },
		{ 0x1A, "ILL", "ILL", 0, 0 },
		{ 0x1B, "ILL", "ILL", 0, 0 },
		{ 0x1C, "ILL", "ILL", 0, 0 },
		{ 0x1D, "ORA", "ABX", 3, 4 },
		{ 0x1E, "ASL", "ABX", 3, 7 },
		{ 0x1F, "ILL", "ILL", 0, 0 },
		{ 0x20, "JSR", "ABS", 3, 6 },
		{ 0x21, "AND", "INX", 2, 6 },
		{ 0x22, "ILL", "ILL", 0, 0 },
		{ 0x23, "ILL", "ILL", 0, 0 },
		{ 0x24, "BIT", "ZPG", 2, 3 },
		{ 0x25, "AND", "ZPG", 2, 3 },
		{ 0x26, "ROL", "ZPG", 2, 5 },
		{ 0x27, "ILL", "ILL", 0, 0 },
		{ 0x28, "PLP", "IMP", 1, 4 },
		{ 0x29, "AND", "IMM", 2, 2 },
		{ 0x2A, "ROL", "ACC", 0, 2 },
		{ 0x2B, "ILL", "ILL", 0, 0 },
		{ 0x2C, "BIT", "ABS", 3, 4 },
		{ 0x2D, "AND", "ABS", 3, 4 },
		{ 0x2E, "ROL", "ABS", 3, 6 },
		{ 0x2F, "ILL", "ILL", 0, 0 },
		{ 0x30, "BMI", "REL", 2, 2 },
		{ 0x31, "AND", "INY", 2, 5 },
		{ 0x32, "ILL", "ILL", 0, 0 },
		{ 0x33, "ILL", "ILL", 0, 0 },
		{ 0x34, "ILL", "ILL", 0, 0 },
		{ 0x35, "AND", "ZPX", 2, 4 },
		{ 0x36, "ROL", "ZPX", 2, 6 },
		{ 0x37, "ILL", "ILL", 0, 0 },
		{ 0x38, "SEC", "IMP", 1, 2 },
		{ 0x39, "AND", "ABY", 3, 4 },
		{ 0x3A, "ILL", "ILL", 0, 0 },
		{ 0x3B, "ILL", "ILL", 0, 0 },
		{ 0x3C, "ILL", "ILL", 0, 0 },
		{ 0x3D, "AND", "ABX", 3, 4 },
		{ 0x3E, "ROL", "ABX", 3, 7 },
		{ 0x3F, "ILL", "ILL", 0, 0 },
		{ 0x40, "RTI", "IMP", 1, 6 },
		{ 0x41, "EOR", "INX", 2, 6 },
		{ 0x42, "ILL", "ILL", 0, 0 },
		{ 0x43, "ILL", "ILL", 0, 0 },
		{ 0x44, "ILL", "ILL", 0, 0 },
		{ 0x45, "EOR", "ZPG", 2, 3 },
		{ 0x46, "LSR", "ZPG", 2, 5 },
		{ 0x47, "ILL", "ILL", 0, 0 },
		{ 0x48, "PHA", "IMP", 1, 3 },
		{ 0x49, "EOR", "IMM", 2, 2 },
		{ 0x4A, "LSR", "ACC", 0, 2 },
		{ 0x4B, "ILL", "ILL", 0, 0 },
		{ 0x4C, "JMP", "ABS", 3, 3 },
		{ 0x4D, "EOR", "ABS", 3, 4 },
		{ 0x4E, "LSR", "ABS", 3, 6 },
		{ 0x4F, "ILL", "ILL", 0, 0 },
		{ 0x50, "BVC", "REL", 2, 2 },
		{ 0x51, "EOR", "INY", 2, 5 },
		{ 0x52, "ILL", "ILL", 0, 0 },
		{ 0x53, "ILL", "ILL", 0, 0 },
		{ 0x54, "ILL", "ILL", 0, 0 },
		{ 0x55, "EOR", "ZPX", 2, 4 },
		{ 0x56, "LSR", "ZPX", 2, 6 },
		{ 0x57, "ILL", "ILL", 0, 0 },
		{ 0x58, "CLI", "IMP", 1, 2 },
		{ 0x59, "EOR", "ABY", 3, 4 },
		{ 0x5A, "ILL", "ILL", 0, 0 },
		{ 0x5B, "ILL", "ILL", 0, 0 },
		{ 0x5C, "ILL", "ILL", 0, 0 },
		{ 0x5D, "EOR", "ABX", 3, 4 },
		{ 0x5E, "LSR", "ABX", 3, 7 },
		{ 0x5F, "ILL", "ILL", 0, 0 },
		{ 0x60, "RTS", "IMP", 1, 6 },
		{ 0x61, "ADC", "INX", 2, 6 },
		{ 0x62, "ILL", "ILL", 0, 0 },
		{ 0x63, "ILL", "ILL", 0, 0 },
		{ 0x64, "ILL", "ILL", 0, 0 },
		{ 0x65, "ADC", "ZPG", 2, 3 },
		{ 0x66, "ROR", "ZPG", 2, 5 },
		{ 0x67, "ILL", "ILL", 0, 0 },
		{ 0x68, "PLA", "IMP", 1, 4 },
		{ 0x69, "ADC", "IMM", 2, 2 },
		{ 0x6A, "ROR", "ACC", 0, 2 },
		{ 0x6B, "ILL", "ILL", 0, 0 },
		{ 0x6C, "JMP", "IND", 3, 5 },
		{ 0x6D, "ADC", "ABS", 3, 4 },
		{ 0x6E, "ROR", "ABS", 3, 6 },
		{ 0x6F, "ILL", "ILL", 0, 0 },
		{ 0x70, "BVC", "REL", 2, 2 },
		{ 0x71, "ADC", "INY", 2, 5 },
		{ 0x72, "ILL", "ILL", 0, 0 },
		{ 0x73, "ILL", "ILL", 0, 0 },
		{ 0x74, "ILL", "ILL", 0, 0 },
		{ 0x75, "ADC", "ZPX", 2, 4 },
		{ 0x76, "ROR", "ZPX", 2, 6 },
		{ 0x77, "ILL", "ILL", 0, 0 },
		{ 0x78, "SEI", "IMP", 1, 2 },
		{ 0x79, "ADC", "ABY", 3, 4 },
		{ 0x7A, "ILL", "ILL", 0, 0 },
		{ 0x7B, "ILL", "ILL", 0, 0 },
		{ 0x7C, "ILL", "ILL", 0, 0 },
		{ 0x7D, "ADC", "ABX", 3, 4 },
		{ 0x7E, "ROR", "ABX", 3, 7 },
		{ 0x7F, "ILL", "ILL", 0, 0 },
		{ 0x80, "ILL", "ILL", 0, 0 },
		{ 0x81, "STA", "INX", 2, 6 },
		{ 0x82, "ILL", "ILL", 0, 0 },
		{ 0x83, "ILL", "ILL", 0, 0 },
		{ 0x84, "STY", "ZPG", 2, 3 },
		{ 0x85, "STA", "ZPG", 2, 3 },
		{ 0x86, "STX", "ZPG", 2, 3 },
		{ 0x87, "ILL", "ILL", 0, 0 },
		{ 0x88, "DEY", "IMP", 1, 2 },
		{ 0x89, "ILL", "ILL", 0, 0 },
		{ 0x8A, "TXA", "IMP", 1, 2 },
		{ 0x8B, "ILL", "ILL", 0, 0 },
		{ 0x8C, "STY", "ABS", 3, 4 },
		{ 0x8D, "STA", "ABS", 3, 4 },
		{ 0x8E, "STX", "ABS", 3, 4 },
		{ 0x8F, "ILL", "ILL", 0, 0 },
		{ 0x90, "BCC", "REL", 2, 2 },
		{ 0x91, "STA", "INY", 2, 6 },
		{ 0x92, "ILL", "ILL", 0, 0 },
		{ 0x93, "ILL", "ILL", 0, 0 },
		{ 0x94, "STY", "ZPX", 2, 4 },
		{ 0x95, "STA", "ZPX", 2, 4 },
		{ 0x96, "STX", "ZPY", 2, 4 },
		{ 0x97, "ILL", "ILL", 0, 0 },
		{ 0x98, "TYA", "IMP", 1, 2 },
		{ 0x99, "STA", "ABY", 3, 5 },
		{ 0x9A, "TXS", "IMP", 1, 2 },
		{ 0x9B, "ILL", "ILL", 0, 0 },
		{ 0x9C, "ILL", "ILL", 0, 0 },
		{ 0x9D, "STA", "ABX", 3, 5 },
		{ 0x9E, "ILL", "ILL", 0, 0 },
		{ 0x9F, "ILL", "ILL", 0, 0 },
		{ 0xA0, "LDY", "IMM", 2, 2 },
		{ 0xA1, "LDA", "INX", 2, 6 },
		{ 0xA2, "LDX", "IMM", 2, 2 },
		{ 0xA3, "ILL", "ILL", 0, 0 },
		{ 0xA4, "LDY", "ZPG", 2, 3 },
		{ 0xA5, "LDA", "ZPG", 2, 3 },
		{ 0xA6, "LDX", "ZPG", 2, 3 },
		{ 0xA7, "ILL", "ILL", 0, 0 },
		{ 0xA8, "TAY", "IMP", 1, 2 },
		{ 0xA9, "LDA", "IMM", 2, 2 },
		{ 0xAA, "TAX", "IMP", 1, 2 },
		{ 0xAB, "ILL", "ILL", 0, 0 },
		{ 0xAC, "LDY", "ABS", 3, 4 },
		{ 0xAD, "LDA", "ABS", 3, 4 },
		{ 0xAE, "LDX", "ABS", 3, 4 },
		{ 0xAF, "ILL", "ILL", 0, 0 },
		{ 0xB0, "BCS", "REL", 2, 2 },
		{ 0xB1, "LDA", "INY", 2, 5 },
		{ 0xB2, "ILL", "ILL", 0, 0 },
		{ 0xB3, "ILL", "ILL", 0, 0 },
		{ 0xB4, "LDY", "ZPX", 2, 4 },
		{ 0xB5, "LDA", "ZPX", 2, 4 },
		{ 0xB6, "LDX", "ZPY", 2, 4 },
		{ 0xB7, "ILL", "ILL", 0, 0 },
		{ 0xB8, "CLV", "IMP", 1, 2 },
		{ 0xB9, "LDA", "ABY", 3, 4 },
		{ 0xBA, "TSX", "IMP", 1, 2 },
		{ 0xBB, "ILL", "ILL", 0, 0 },
		{ 0xBC, "LDY", "ABX", 3, 4 },
		{ 0xBD, "LDA", "ABX", 3, 4 },
		{ 0xBE, "LDX", "ABY", 3, 4 },
		{ 0xBF, "ILL", "ILL", 0, 0 },
		{ 0xC0, "CPY", "IMM", 2, 2 },
		{ 0xC1, "CMP", "INX", 2, 6 },
		{ 0xC2, "ILL", "ILL", 0, 0 },
		{ 0xC3, "ILL", "ILL", 0, 0 },
		{ 0xC4, "CPY", "ZPG", 2, 3 },
		{ 0xC5, "CMP", "ZPG", 2, 3 },
		{ 0xC6, "DEC", "ZPG", 2, 5 },
		{ 0xC7, "ILL", "ILL", 0, 0 },
		{ 0xC8, "INY", "IMP", 1, 2 },
		{ 0xC9, "CMP", "IMM", 2, 2 },
		{ 0xCA, "DEX", "IMP", 1, 2 },
		{ 0xCB, "ILL", "ILL", 0, 0 },
		{ 0xCC, "CPY", "ABS", 3, 4 },
		{ 0xCD, "CMP", "ABS", 3, 4 },
		{ 0xCE, "DEC", "ABS", 3, 6 },
		{ 0xCF, "ILL", "ILL", 0, 0 },
		{ 0xD0, "BNE", "REL", 2, 2 },
		{ 0xD1, "CMP", "INY", 2, 5 },
		{ 0xD2, "ILL", "ILL", 0, 0 },
		{ 0xD3, "ILL", "ILL", 0, 0 },
		{ 0xD4, "ILL", "ILL", 0, 0 },
		{ 0xD5, "CMP", "ZPX", 2, 4 },
		{ 0xD6, "DEC", "ZPX", 2, 6 },
		{ 0xD7, "ILL", "ILL", 0, 0 },
		{ 0xD8, "CLD", "IMP", 1, 2 },
		{ 0xD9, "CMP", "ABY", 3, 4 },
		{ 0xDA, "ILL", "ILL", 0, 0 },
		{ 0xDB, "ILL", "ILL", 0, 0 },
		{ 0xDC, "ILL", "ILL", 0, 0 },
		{ 0xDD, "CMP", "ABX", 3, 4 },
		{ 0xDE, "DEC", "ABX", 3, 7 },
		{ 0xDF, "ILL", "ILL", 0, 0 },
		{ 0xE0, "CPX", "IMM", 2, 2 },
		{ 0xE1, "SBC", "INX", 2, 6 },
		{ 0xE2, "ILL", "ILL", 0, 0 },
		{ 0xE3, "ILL", "ILL", 0, 0 },
		{ 0xE4, "CPX", "ZPG", 2, 3 },
		{ 0xE5, "SBC", "ZPG", 2, 3 },
		{ 0xE6, "INC", "ZPG", 2, 5 },
		{ 0xE7, "ILL", "ILL", 0, 0 },
		{ 0xE8, "INX", "IMP", 1, 2 },
		{ 0xE9, "SBC", "IMM", 2, 2 },
		{ 0xEA, "NOP", "IMP", 1, 2 },
		{ 0xEB, "ILL", "ILL", 0, 0 },
		{ 0xEC, "CPX", "ABS", 3, 4 },
		{ 0xED, "SBC", "ABS", 3, 4 },
		{ 0xEE, "INC", "ABS", 3, 6 },
		{ 0xEF, "ILL", "ILL", 0, 0 },
		{ 0xF0, "BEQ", "REL", 2, 2 },
		{ 0xF1, "SBC", "INY", 2, 5 },
		{ 0xF2, "ILL", "ILL", 0, 0 },
		{ 0xF3, "ILL", "ILL", 0, 0 },
		{ 0xF4, "ILL", "ILL", 0, 0 },
		{ 0xF5, "SBC", "ZPX", 2, 4 },
		{ 0xF6, "INC", "ZPX", 2, 6 },
		{ 0xF7, "ILL", "ILL", 0, 0 },
		{ 0xF8, "SED", "IMP", 1, 2 },
		{ 0xF9, "SBC", "ABY", 3, 4 },
		{ 0xFA, "ILL", "ILL", 0, 0 },
		{ 0xFB, "ILL", "ILL", 0, 0 },
		{ 0xFC, "ILL", "ILL", 0, 0 },
		{ 0xFD, "SBC", "ABX", 3, 4 },
		{ 0xFE, "INC", "ABX", 3, 7 },
	};;

	// addressing modes
	std::uint8_t ABS();
	std::uint8_t ABX();
	std::uint8_t ABY();
	std::uint8_t ACC();
	std::uint8_t IND();
	std::uint8_t IZX();
	std::uint8_t IZY();
	std::uint8_t IMP();
	std::uint8_t IMM();
	std::uint8_t ZPG();
	std::uint8_t ZPX();
	std::uint8_t ZPY();
	std::uint8_t REL();

	// instructions
	std::uint8_t ADC();
	std::uint8_t AND();
	std::uint8_t ASL();
	std::uint8_t BCC();
	std::uint8_t BCS();
	std::uint8_t BEQ();
	std::uint8_t BIT();
	std::uint8_t BMI();
	std::uint8_t BNE();
	std::uint8_t BPL();
	std::uint8_t BRK();
	std::uint8_t BVC();
	std::uint8_t BVS();
	std::uint8_t CLC();
	std::uint8_t CLD();
	std::uint8_t CLI();
	std::uint8_t CLV();
	std::uint8_t CMP();
	std::uint8_t CPX();
	std::uint8_t CPY();
	std::uint8_t DEC();
	std::uint8_t DEX();
	std::uint8_t DEY();
	std::uint8_t EOR();
	std::uint8_t INC();
	std::uint8_t INX();
	std::uint8_t INY();
	std::uint8_t JMP();
	std::uint8_t JSR();
	std::uint8_t LDA();
	std::uint8_t LDX();
	std::uint8_t LDY();
	std::uint8_t LSR();
	std::uint8_t NOP();
	std::uint8_t ORA();
	std::uint8_t PHA();
	std::uint8_t PHP();
	std::uint8_t PLA();
	std::uint8_t PLP();
	std::uint8_t ROL();
	std::uint8_t ROR();
	std::uint8_t RTI();
	std::uint8_t RTS();
	std::uint8_t SBC();
	std::uint8_t SEC();
	std::uint8_t SED();
	std::uint8_t SEI();
	std::uint8_t STA();
	std::uint8_t STX();
	std::uint8_t STY();
	std::uint8_t TAX();
	std::uint8_t TAY();
	std::uint8_t TSX();
	std::uint8_t TXA();
	std::uint8_t TXS();
	std::uint8_t TYA();

	std::uint8_t ILL(); // illegal instruction
};

#endif
