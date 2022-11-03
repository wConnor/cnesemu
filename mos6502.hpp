#ifndef MOS6502_HPP
#define MOS6502_HPP

#include <bitset>
#include <string>
#include <stack>
#include <array>
#include <cstdint>
#include <algorithm>

constexpr std::uint16_t MEM_SIZE = 2048;
constexpr std::uint8_t STACK_SIZE = 255;
constexpr std::uint8_t INSTRUCTION_COUNT = 255;

class MOS6502
{
public:
	/* ADDRMODE & INSTRUCTION source:
	   https://www.masswerk.at/6502/6502_instruction_set.html */
	enum class ADDRMODE : std::uint8_t {
		ABS, // absolute
		ABX, // absolute, offset by index X
		ABY, // absolute, offset by index Y
		ACC, // accumulator
		IMM, // immediate
		IMP, // implied
		IND, // indirect
		INX, // indirect, offset by index X
		INY, // indirect, offset by index Y
		REL, // relative
		ZPG, // zeropage
		ZPX, // zeropage, offset by index X
		ZPY, // zeropage, offset by index Y
	};

	enum class INSTRUCTION : std::uint8_t {
		ADC, // add with carry
		AND, // AND with ACC
		ASL, // arithmetic shift left
		BCC, // branch on carry clear
		BCS, // branch on carry set
		BEQ, // branch on equal (zero set)
		BIT, // bit test
		BMI, // branch on minus (negative set)
		BNE, // branch on not equal (zero clear)
		BPL, // branch on plus (negative clear)
		BRK, // break / interrupt
		BVC, // branch on overflow clear
		BVS, // branch on overflow set
		CLC, // clear carry
		CLD, // clear decimal
		CLI, // clear interrupt disable
		CLV, // clear overflow
		CMP, // compare with ACC
		CPX, // compare with index X
		CPY, // compare with index Y
		DEC, // decrement
		DEX, // decrement index X
		DEY, // decrement index Y
		EOR, // XOR with ACC
		INC, // increment
		INX, // increment index X
		INY, // increment index Y
		JMP, // jump
		JSR, // jump subroutine
		LDA, // load ACC
		LDX, // load index X
		LDY, // load index Y
		LSR, // logical shift right
		NOP, // no operation
		ORA, // OR with ACC
		PHA, // push ACC
		PHP, // push status register (SR)
		PLA, // pull ACC
		PLP, // pull status register (SR)
		ROL, // rotate left
		ROR, // rotate right
		RTI, // return from interrupt
		RTS, // return from subroutine
		SBC, // subtract with carry
		SEC, // set carry
		SED, // set decimal
		SEI, // set interrupt disable
		STA, // store ACC
		STX, // store index X
		STY, // store index Y
		TAX, // transfer ACC to index X
		TAY, // transfer ACC to index Y
		TSX, // transfer SP to index X
		TXA, // transfer index X to ACC
		TXS, // transfer index X to SP
		TYA, // transfer index Y to ACC
	};

	struct INSTRUCTIONINFO {
		std::uint8_t op_code;
		INSTRUCTION instr;
		ADDRMODE addr_mode;
	};

	MOS6502();
	INSTRUCTIONINFO fetch_instruction(const std::uint8_t &op_code);
	void decode_instruction(const INSTRUCTIONINFO &full_instruction);
	void exec_instruction(const std::uint8_t &instruction);
	virtual ~MOS6502();


private:
	/* registers: accumulator, x index, y index, stack pointer, status register, program counter
	   status register: bit 5 can not be changed and is always set to 1.
	   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	   | N | V | - | B | D | I | Z | C | */
	std::uint8_t acc, ix, iy, sp, sr;
	std::uint16_t pc;

	std::stack<std::uint8_t> stack;
	std::array<std::uint8_t, MEM_SIZE> mem;

	const std::array<INSTRUCTIONINFO, INSTRUCTION_COUNT> instr_matrix = {
		INSTRUCTIONINFO
		{ 0x00, INSTRUCTION::BRK, ADDRMODE::IMP },
		{ 0x01, INSTRUCTION::ORA, ADDRMODE::INX },
		//		{ 0x02, nullptr, nullptr },
		//		{ 0x03, nullptr, nullptr },
		//		{ 0x04, nullptr, nullptr },
		{ 0x05, INSTRUCTION::ORA, ADDRMODE::ZPG },
		{ 0x06, INSTRUCTION::ASL, ADDRMODE::ZPG },
		//		{ 0x07, nullptr, nullptr },
		{ 0x08, INSTRUCTION::PHP, ADDRMODE::IMP },
		{ 0x09, INSTRUCTION::ORA, ADDRMODE::IMM },
		{ 0x0A, INSTRUCTION::ASL, ADDRMODE::ACC },
		//		{ 0x0B, nullptr, nullptr },
		//		{ 0x0C, nullptr, nullptr },
		{ 0x0D, INSTRUCTION::ORA, ADDRMODE::ABS },
		{ 0x0E, INSTRUCTION::ASL, ADDRMODE::ABS },
		//		{ 0x0F, nullptr, nullptr },
		{ 0x10, INSTRUCTION::BPL, ADDRMODE::REL },
		{ 0x11, INSTRUCTION::ORA, ADDRMODE::INY },
		//		{ 0x12, nullptr, nullptr },
		//		{ 0x13, nullptr, nullptr },
		//		{ 0x14, nullptr, nullptr },
		{ 0x15, INSTRUCTION::ORA, ADDRMODE::ZPX },
		{ 0x16, INSTRUCTION::ASL, ADDRMODE::ZPX },
		//		{ 0x17, nullptr, nullptr },
		{ 0x18, INSTRUCTION::CLC, ADDRMODE::IMP },
		{ 0x19, INSTRUCTION::ORA, ADDRMODE::ABY },
		//		{ 0x1A, nullptr, nullptr },
		//		{ 0x1B, nullptr, nullptr },
		//		{ 0x1C, nullptr, nullptr },
		{ 0x1D, INSTRUCTION::ORA, ADDRMODE::ABX },
		{ 0x1E, INSTRUCTION::ASL, ADDRMODE::ABX },
		//		{ 0x1F, nullptr, nullptr },
		{ 0x20, INSTRUCTION::JSR, ADDRMODE::ABS },
		{ 0x21, INSTRUCTION::AND, ADDRMODE::INX },
		//		{ 0x22, nullptr, nullptr },
		//		{ 0x23, nullptr, nullptr },
		{ 0x24, INSTRUCTION::BIT, ADDRMODE::ZPG },
		{ 0x25, INSTRUCTION::AND, ADDRMODE::ZPG },
		{ 0x26, INSTRUCTION::ROL, ADDRMODE::ZPG },
		//		{ 0x27, nullptr, nullptr },
		{ 0x28, INSTRUCTION::PLP, ADDRMODE::IMP },
		{ 0x29, INSTRUCTION::AND, ADDRMODE::IMM },
		{ 0x2A, INSTRUCTION::ROL, ADDRMODE::ACC },
		//		{ 0x2B, nullptr, nullptr },
		{ 0x2C, INSTRUCTION::BIT, ADDRMODE::ABS },
		{ 0x2D, INSTRUCTION::AND, ADDRMODE::ABS },
		{ 0x2E, INSTRUCTION::ROL, ADDRMODE::ABS },
		//		{ 0x2F, nullptr, nullptr },
		{ 0x30, INSTRUCTION::BMI, ADDRMODE::REL },
		{ 0x31, INSTRUCTION::AND, ADDRMODE::INY },
		//		{ 0x32, nullptr, nullptr },
		//		{ 0x33, nullptr, nullptr },
		//		{ 0x34, nullptr, nullptr },
		{ 0x35, INSTRUCTION::AND, ADDRMODE::ZPX },
		{ 0x36, INSTRUCTION::ROL, ADDRMODE::ZPX },
		//		{ 0x37, nullptr, nullptr },
		{ 0x38, INSTRUCTION::SEC, ADDRMODE::IMP },
		{ 0x39, INSTRUCTION::AND, ADDRMODE::ABY },
		//		{ 0x3A, nullptr, nullptr },
		//		{ 0x3B, nullptr, nullptr },
		//		{ 0x3C, nullptr, nullptr },
		{ 0x3D, INSTRUCTION::AND, ADDRMODE::ABX },
		{ 0x3E, INSTRUCTION::ROL, ADDRMODE::ABX },
		//		{ 0x3F, nullptr, nullptr },
		{ 0x40, INSTRUCTION::RTI, ADDRMODE::IMP },
		{ 0x41, INSTRUCTION::EOR, ADDRMODE::INX },
		//		{ 0x42, nullptr, nullptr },
		//		{ 0x43, nullptr, nullptr },
		//		{ 0x44, nullptr, nullptr },
		{ 0x45, INSTRUCTION::EOR, ADDRMODE::ZPG },
		{ 0x46, INSTRUCTION::LSR, ADDRMODE::ZPG },
		//		{ 0x47, nullptr, nullptr },
		{ 0x48, INSTRUCTION::PHA, ADDRMODE::IMP },
		{ 0x49, INSTRUCTION::EOR, ADDRMODE::IMM },
		{ 0x4A, INSTRUCTION::LSR, ADDRMODE::ACC },
		//		{ 0x4B, nullptr, nullptr },
		{ 0x4C, INSTRUCTION::JMP, ADDRMODE::ABS },
		{ 0x4D, INSTRUCTION::EOR, ADDRMODE::ABS },
		{ 0x4E, INSTRUCTION::LSR, ADDRMODE::ABS },
		//		{ 0x4F, nullptr, nullptr },
		{ 0x50, INSTRUCTION::BVC, ADDRMODE::REL },
		{ 0x51, INSTRUCTION::EOR, ADDRMODE::INY },
		// { 0x52, nullptr, nullptr },
		// { 0x53, nullptr, nullptr },
		// { 0x54, nullptr, nullptr },
		{ 0x55, INSTRUCTION::EOR, ADDRMODE::ZPX },
		{ 0x56, INSTRUCTION::LSR, ADDRMODE::ZPX },
		// { 0x57, nullptr, nullptr },
		{ 0x58, INSTRUCTION::CLI, ADDRMODE::IMP },
		{ 0x59, INSTRUCTION::EOR, ADDRMODE::ABY },
		// { 0x5A, nullptr, nullptr },
		// { 0x5B, nullptr, nullptr },
		// { 0x5C, nullptr, nullptr },
		{ 0x5D, INSTRUCTION::EOR, ADDRMODE::ABX },
		{ 0x5E, INSTRUCTION::LSR, ADDRMODE::ABX },
		// { 0x5F, nullptr, nullptr },
		{ 0x60, INSTRUCTION::RTS, ADDRMODE::IMP },
		{ 0x61, INSTRUCTION::ADC, ADDRMODE::INX },
		// { 0x62, nullptr, nullptr },
		// { 0x63, nullptr, nullptr },
		// { 0x64, nullptr, nullptr },
		{ 0x65, INSTRUCTION::ADC, ADDRMODE::ZPG },
		{ 0x66, INSTRUCTION::ROR, ADDRMODE::ZPG },
		// { 0x67, nullptr, nullptr },
		{ 0x68, INSTRUCTION::PLA, ADDRMODE::IMP },
		{ 0x69, INSTRUCTION::ADC, ADDRMODE::IMM },
		{ 0x6A, INSTRUCTION::ROR, ADDRMODE::ACC },
		// { 0x6B, nullptr, nullptr },
		{ 0x6C, INSTRUCTION::JMP, ADDRMODE::IND },
		{ 0x6D, INSTRUCTION::ADC, ADDRMODE::ABS },
		{ 0x6E, INSTRUCTION::ROR, ADDRMODE::ABS },
		// { 0x6F, nullptr, nullptr },
		{ 0x70, INSTRUCTION::BVC, ADDRMODE::REL },
		{ 0x71, INSTRUCTION::ADC, ADDRMODE::INY },
		// { 0x72, nullptr, nullptr },
		// { 0x73, nullptr, nullptr },
		// { 0x74, nullptr, nullptr },
		{ 0x75, INSTRUCTION::ADC, ADDRMODE::ZPX },
		{ 0x76, INSTRUCTION::ROR, ADDRMODE::ZPX },
		// { 0x77, nullptr, nullptr },
		{ 0x78, INSTRUCTION::SEI, ADDRMODE::IMP },
		{ 0x79, INSTRUCTION::ADC, ADDRMODE::ABY },
		// { 0x7A, nullptr, nullptr },
		// { 0x7B, nullptr, nullptr },
		// { 0x7C, nullptr, nullptr },
		{ 0x7D, INSTRUCTION::ADC, ADDRMODE::ABX },
		{ 0x7E, INSTRUCTION::ROR, ADDRMODE::ABX },
		// { 0x7F, nullptr, nullptr },
		// { 0x80, nullptr, nullptr },
		{ 0x81, INSTRUCTION::STA, ADDRMODE::INX },
		// { 0x82, nullptr, nullptr },
		// { 0x83, nullptr, nullptr },
		{ 0x84, INSTRUCTION::STY, ADDRMODE::ZPG },
		{ 0x85, INSTRUCTION::STA, ADDRMODE::ZPG },
		{ 0x86, INSTRUCTION::STX, ADDRMODE::ZPG },
		// { 0x87, nullptr, nullptr },
		{ 0x88, INSTRUCTION::DEY, ADDRMODE::IMP },
		// { 0x89, nullptr, nullptr },
		{ 0x8A, INSTRUCTION::TXA, ADDRMODE::IMP },
		// { 0x8B, nullptr, nullptr },
		{ 0x8C, INSTRUCTION::STY, ADDRMODE::ABS },
		{ 0x8D, INSTRUCTION::STA, ADDRMODE::ABS },
		{ 0x8E, INSTRUCTION::STX, ADDRMODE::ABS },
		// { 0x8F, nullptr, nullptr },
		{ 0x90, INSTRUCTION::BCC, ADDRMODE::REL },
		{ 0x91, INSTRUCTION::STA, ADDRMODE::INY },
		// { 0x92, nullptr, nullptr },
		// { 0x93, nullptr, nullptr },
		{ 0x94, INSTRUCTION::STY, ADDRMODE::ZPX },
		{ 0x95, INSTRUCTION::STA, ADDRMODE::ZPX },
		{ 0x96, INSTRUCTION::STX, ADDRMODE::ZPY },
		// { 0x97, nullptr, nullptr },
		{ 0x98, INSTRUCTION::TYA, ADDRMODE::IMP },
		{ 0x99, INSTRUCTION::STA, ADDRMODE::ABY },
		{ 0x9A, INSTRUCTION::TXS, ADDRMODE::IMP },
		// { 0x9B, nullptr, nullptr },
		// { 0x9C, nullptr, nullptr },
		{ 0x9D, INSTRUCTION::STA, ADDRMODE::ABX },
		// { 0x9E, nullptr, nullptr },
		// { 0x9F, nullptr, nullptr },
		{ 0xA0, INSTRUCTION::LDY, ADDRMODE::IMM },
		{ 0xA1, INSTRUCTION::LDA, ADDRMODE::INX },
		{ 0xA2, INSTRUCTION::LDX, ADDRMODE::IMM },
		// { 0xa3, nullptr, nullptr },
		{ 0xA4, INSTRUCTION::LDY, ADDRMODE::ZPG },
		{ 0xA5, INSTRUCTION::LDA, ADDRMODE::ZPG },
		{ 0xA6, INSTRUCTION::LDX, ADDRMODE::ZPG },
		// { 0xa7, nullptr, nullptr },
		{ 0xA8, INSTRUCTION::TAY, ADDRMODE::IMP },
		{ 0xA9, INSTRUCTION::LDA, ADDRMODE::IMM },
		{ 0xAA, INSTRUCTION::TAX, ADDRMODE::IMP },
		// { 0xAB, nullptr, nullptr },
		{ 0xAC, INSTRUCTION::LDY, ADDRMODE::ABS },
		{ 0xAD, INSTRUCTION::LDA, ADDRMODE::ABS },
		{ 0xAE, INSTRUCTION::LDX, ADDRMODE::ABS },
		// { 0xAF, nullptr, nullptr },
		{ 0xB0, INSTRUCTION::BCS, ADDRMODE::REL },
		{ 0xB1, INSTRUCTION::LDA, ADDRMODE::INY },
		// { 0xB2, nullptr, nullptr },
		// { 0xB3, nullptr, nullptr },
		{ 0xB4, INSTRUCTION::LDY, ADDRMODE::ZPX },
		{ 0xB5, INSTRUCTION::LDA, ADDRMODE::ZPX },
		{ 0xB6, INSTRUCTION::LDX, ADDRMODE::ZPY },
		// { 0xB7, nullptr, nullptr },
		{ 0xB8, INSTRUCTION::CLV, ADDRMODE::IMP },
		{ 0xB9, INSTRUCTION::LDA, ADDRMODE::ABY },
		{ 0xBA, INSTRUCTION::TSX, ADDRMODE::IMP },
		// { 0xBB, nullptr, nullptr },
		{ 0xBC, INSTRUCTION::LDY, ADDRMODE::ABX },
		{ 0xBD, INSTRUCTION::LDA, ADDRMODE::ABX },
		{ 0xBE, INSTRUCTION::LDX, ADDRMODE::ABY },
		// { 0xBF, nullptr, nullptr },
		{ 0xC0, INSTRUCTION::CPY, ADDRMODE::IMM },
		{ 0xC1, INSTRUCTION::CMP, ADDRMODE::INX },
		// { 0xC2, nullptr, nullptr },
		// { 0xC3, nullptr, nullptr },
		{ 0xC4, INSTRUCTION::CPY, ADDRMODE::ZPG },
		{ 0xC5, INSTRUCTION::CMP, ADDRMODE::ZPG },
		{ 0xC6, INSTRUCTION::DEC, ADDRMODE::ZPG },
		// { 0xC7, nullptr, nullptr },
		{ 0xC8, INSTRUCTION::INY, ADDRMODE::IMP },
		{ 0xC9, INSTRUCTION::CMP, ADDRMODE::IMM },
		{ 0xCA, INSTRUCTION::DEX, ADDRMODE::IMP },
		// { 0xCB, nullptr, nullptr },
		{ 0xCC, INSTRUCTION::CPY, ADDRMODE::ABS },
		{ 0xCD, INSTRUCTION::CMP, ADDRMODE::ABS },
		{ 0xCE, INSTRUCTION::DEC, ADDRMODE::ABS },
		// { 0xCF, nullptr, nullptr },
		{ 0xD0, INSTRUCTION::BNE, ADDRMODE::REL },
		{ 0xD1, INSTRUCTION::CMP, ADDRMODE::INY },
		// { 0xD2, nullptr, nullptr },
		// { 0xD3, nullptr, nullptr },
		// { 0xD4, nullptr, nullptr },
		{ 0xD5, INSTRUCTION::CMP, ADDRMODE::ZPX },
		{ 0xD6, INSTRUCTION::DEC, ADDRMODE::ZPX },
		// { 0xD7, nullptr, nullptr },
		{ 0xD8, INSTRUCTION::CLD, ADDRMODE::IMP },
		{ 0xD9, INSTRUCTION::CMP, ADDRMODE::ABY },
		// { 0xDA, nullptr, nullptr },
		// { 0xDB, nullptr, nullptr },
		// { 0xDC, nullptr, nullptr },
		{ 0xDD, INSTRUCTION::CMP, ADDRMODE::ABX },
		{ 0xDE, INSTRUCTION::DEC, ADDRMODE::ABX },
		// { 0xDF, nullptr, nullptr },
		{ 0xE0, INSTRUCTION::CPX, ADDRMODE::IMM },
		{ 0xE1, INSTRUCTION::SBC, ADDRMODE::INX },
		// { 0xE2, nullptr, nullptr },
		// { 0xE3, nullptr, nullptr },
		{ 0xE4, INSTRUCTION::CPX, ADDRMODE::ZPG },
		{ 0xE5, INSTRUCTION::SBC, ADDRMODE::ZPG },
		{ 0xE6, INSTRUCTION::INC, ADDRMODE::ZPG },
		// { 0xE7, nullptr, nullptr },
		{ 0xE8, INSTRUCTION::INX, ADDRMODE::IMP },
		{ 0xE9, INSTRUCTION::SBC, ADDRMODE::IMM },
		{ 0xEA, INSTRUCTION::NOP, ADDRMODE::IMP },
		// { 0xEB, nullptr, nullptr },
		{ 0xEC, INSTRUCTION::CPX, ADDRMODE::ABS },
		{ 0xED, INSTRUCTION::SBC, ADDRMODE::ABS },
		{ 0xEE, INSTRUCTION::INC, ADDRMODE::ABS },
		// { 0xEF, nullptr, nullptr },
		{ 0xF0, INSTRUCTION::BEQ, ADDRMODE::REL },
		{ 0xF1, INSTRUCTION::SBC, ADDRMODE::INY },
		// { 0xF2, nullptr, nullptr },
		// { 0xF3, nullptr, nullptr },
		// { 0xF4, nullptr, nullptr },
		{ 0xF5, INSTRUCTION::SBC, ADDRMODE::ZPX },
		{ 0xF6, INSTRUCTION::INC, ADDRMODE::ZPX },
		// { 0xF7, nullptr, nullptr },
		{ 0xF8, INSTRUCTION::SED, ADDRMODE::IMP },
		{ 0xF9, INSTRUCTION::SBC, ADDRMODE::ABY },
		// { 0xFA, nullptr, nullptr },
		// { 0xFB, nullptr, nullptr },
		// { 0xFC, nullptr, nullptr },
		{ 0xFD, INSTRUCTION::SBC, ADDRMODE::ABX },
		{ 0xFE, INSTRUCTION::INC, ADDRMODE::ABX },
	};
};

#endif
