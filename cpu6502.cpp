#include "cpu6502.hpp"

CPU6502::CPU6502()
{
	/* instruction matrix from https://www.masswerk.at/6502/6502_instruction_set.html
	   lines that are commented out are illegal. likely to be replaced with
	   INSTRUCTION::ILL and ADDRMODE::ILL in the future. */
	this->instr_matrix = {
		FULLINSTRUCTION
		{ 0x00, INSTRUCTION::BRK, ADDRMODE::IMP, 1, 7 },
		{ 0x01, INSTRUCTION::ORA, ADDRMODE::INX, 2, 6 },
		{ 0x02, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x03, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x04, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x05, INSTRUCTION::ORA, ADDRMODE::ZPG, 2, 3 },
		{ 0x06, INSTRUCTION::ASL, ADDRMODE::ZPG, 2, 5 },
		{ 0x07, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x08, INSTRUCTION::PHP, ADDRMODE::IMP, 1, 3 },
		{ 0x09, INSTRUCTION::ORA, ADDRMODE::IMM, 2, 2 },
		{ 0x0A, INSTRUCTION::ASL, ADDRMODE::ACC, 0, 2 },
		{ 0x0B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x0C, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x0D, INSTRUCTION::ORA, ADDRMODE::ABS, 3, 4 },
		{ 0x0E, INSTRUCTION::ASL, ADDRMODE::ABS, 3, 6 },
		{ 0x0F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x10, INSTRUCTION::BPL, ADDRMODE::REL, 2, 2 },
		{ 0x11, INSTRUCTION::ORA, ADDRMODE::INY, 2, 5 },
		{ 0x12, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x13, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x14, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x15, INSTRUCTION::ORA, ADDRMODE::ZPX, 2, 4 },
		{ 0x16, INSTRUCTION::ASL, ADDRMODE::ZPX, 2, 6 },
		{ 0x17, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x18, INSTRUCTION::CLC, ADDRMODE::IMP, 1, 2 },
		{ 0x19, INSTRUCTION::ORA, ADDRMODE::ABY, 3, 4 },
		{ 0x1A, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x1B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x1C, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x1D, INSTRUCTION::ORA, ADDRMODE::ABX, 3, 4 },
		{ 0x1E, INSTRUCTION::ASL, ADDRMODE::ABX, 3, 7 },
		{ 0x1F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x20, INSTRUCTION::JSR, ADDRMODE::ABS, 3, 6 },
		{ 0x21, INSTRUCTION::AND, ADDRMODE::INX, 2, 6 },
		{ 0x22, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x23, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x24, INSTRUCTION::BIT, ADDRMODE::ZPG, 2, 3 },
		{ 0x25, INSTRUCTION::AND, ADDRMODE::ZPG, 2, 3 },
		{ 0x26, INSTRUCTION::ROL, ADDRMODE::ZPG, 2, 5 },
		{ 0x27, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x28, INSTRUCTION::PLP, ADDRMODE::IMP, 1, 4 },
		{ 0x29, INSTRUCTION::AND, ADDRMODE::IMM, 2, 2 },
		{ 0x2A, INSTRUCTION::ROL, ADDRMODE::ACC, 0, 2 },
		{ 0x2B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x2C, INSTRUCTION::BIT, ADDRMODE::ABS, 3, 4 },
		{ 0x2D, INSTRUCTION::AND, ADDRMODE::ABS, 3, 4 },
		{ 0x2E, INSTRUCTION::ROL, ADDRMODE::ABS, 3, 6 },
		{ 0x2F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x30, INSTRUCTION::BMI, ADDRMODE::REL, 2, 2 },
		{ 0x31, INSTRUCTION::AND, ADDRMODE::INY, 2, 5 },
		{ 0x32, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x33, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x34, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x35, INSTRUCTION::AND, ADDRMODE::ZPX, 2, 4 },
		{ 0x36, INSTRUCTION::ROL, ADDRMODE::ZPX, 2, 6 },
		{ 0x37, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x38, INSTRUCTION::SEC, ADDRMODE::IMP, 1, 2 },
		{ 0x39, INSTRUCTION::AND, ADDRMODE::ABY, 3, 4 },
		{ 0x3A, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x3B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x3C, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x3D, INSTRUCTION::AND, ADDRMODE::ABX, 3, 4 },
		{ 0x3E, INSTRUCTION::ROL, ADDRMODE::ABX, 3, 7 },
		{ 0x3F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x40, INSTRUCTION::RTI, ADDRMODE::IMP, 1, 6 },
		{ 0x41, INSTRUCTION::EOR, ADDRMODE::INX, 2, 6 },
		{ 0x42, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x43, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x44, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x45, INSTRUCTION::EOR, ADDRMODE::ZPG, 2, 3 },
		{ 0x46, INSTRUCTION::LSR, ADDRMODE::ZPG, 2, 5 },
		{ 0x47, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x48, INSTRUCTION::PHA, ADDRMODE::IMP, 1, 3 },
		{ 0x49, INSTRUCTION::EOR, ADDRMODE::IMM, 2, 2 },
		{ 0x4A, INSTRUCTION::LSR, ADDRMODE::ACC, 0, 2 },
		{ 0x4B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x4C, INSTRUCTION::JMP, ADDRMODE::ABS, 3, 3 },
		{ 0x4D, INSTRUCTION::EOR, ADDRMODE::ABS, 3, 4 },
		{ 0x4E, INSTRUCTION::LSR, ADDRMODE::ABS, 3, 6 },
		{ 0x4F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x50, INSTRUCTION::BVC, ADDRMODE::REL, 2, 2 },
		{ 0x51, INSTRUCTION::EOR, ADDRMODE::INY, 2, 5 },
		{ 0x52, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x53, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x54, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x55, INSTRUCTION::EOR, ADDRMODE::ZPX, 2, 4 },
		{ 0x56, INSTRUCTION::LSR, ADDRMODE::ZPX, 2, 6 },
		{ 0x57, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x58, INSTRUCTION::CLI, ADDRMODE::IMP, 1, 2 },
		{ 0x59, INSTRUCTION::EOR, ADDRMODE::ABY, 3, 4 },
		{ 0x5A, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x5B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x5C, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x5D, INSTRUCTION::EOR, ADDRMODE::ABX, 3, 4 },
		{ 0x5E, INSTRUCTION::LSR, ADDRMODE::ABX, 3, 7 },
		{ 0x5F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x60, INSTRUCTION::RTS, ADDRMODE::IMP, 1, 6 },
		{ 0x61, INSTRUCTION::ADC, ADDRMODE::INX, 2, 6 },
		{ 0x62, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x63, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x64, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x65, INSTRUCTION::ADC, ADDRMODE::ZPG, 2, 3 },
		{ 0x66, INSTRUCTION::ROR, ADDRMODE::ZPG, 2, 5 },
		{ 0x67, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x68, INSTRUCTION::PLA, ADDRMODE::IMP, 1, 4 },
		{ 0x69, INSTRUCTION::ADC, ADDRMODE::IMM, 2, 2 },
		{ 0x6A, INSTRUCTION::ROR, ADDRMODE::ACC, 0, 2 },
		{ 0x6B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x6C, INSTRUCTION::JMP, ADDRMODE::IND, 3, 5 },
		{ 0x6D, INSTRUCTION::ADC, ADDRMODE::ABS, 3, 4 },
		{ 0x6E, INSTRUCTION::ROR, ADDRMODE::ABS, 3, 6 },
		{ 0x6F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x70, INSTRUCTION::BVC, ADDRMODE::REL, 2, 2 },
		{ 0x71, INSTRUCTION::ADC, ADDRMODE::INY, 2, 5 },
		{ 0x72, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x73, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x74, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x75, INSTRUCTION::ADC, ADDRMODE::ZPX, 2, 4 },
		{ 0x76, INSTRUCTION::ROR, ADDRMODE::ZPX, 2, 6 },
		{ 0x77, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x78, INSTRUCTION::SEI, ADDRMODE::IMP, 1, 2 },
		{ 0x79, INSTRUCTION::ADC, ADDRMODE::ABY, 3, 4 },
		{ 0x7A, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x7B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x7C, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x7D, INSTRUCTION::ADC, ADDRMODE::ABX, 3, 4 },
		{ 0x7E, INSTRUCTION::ROR, ADDRMODE::ABX, 3, 7 },
		{ 0x7F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x80, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x81, INSTRUCTION::STA, ADDRMODE::INX, 2, 6 },
		{ 0x82, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x83, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x84, INSTRUCTION::STY, ADDRMODE::ZPG, 2, 3 },
		{ 0x85, INSTRUCTION::STA, ADDRMODE::ZPG, 2, 3 },
		{ 0x86, INSTRUCTION::STX, ADDRMODE::ZPG, 2, 3 },
		{ 0x87, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x88, INSTRUCTION::DEY, ADDRMODE::IMP, 1, 2 },
		{ 0x89, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x8A, INSTRUCTION::TXA, ADDRMODE::IMP, 1, 2 },
		{ 0x8B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x8C, INSTRUCTION::STY, ADDRMODE::ABS, 3, 4 },
		{ 0x8D, INSTRUCTION::STA, ADDRMODE::ABS, 3, 4 },
		{ 0x8E, INSTRUCTION::STX, ADDRMODE::ABS, 3, 4 },
		{ 0x8F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x90, INSTRUCTION::BCC, ADDRMODE::REL, 2, 2 },
		{ 0x91, INSTRUCTION::STA, ADDRMODE::INY, 2, 6 },
		{ 0x92, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x93, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x94, INSTRUCTION::STY, ADDRMODE::ZPX, 2, 4 },
		{ 0x95, INSTRUCTION::STA, ADDRMODE::ZPX, 2, 4 },
		{ 0x96, INSTRUCTION::STX, ADDRMODE::ZPY, 2, 4 },
		{ 0x97, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x98, INSTRUCTION::TYA, ADDRMODE::IMP, 1, 2 },
		{ 0x99, INSTRUCTION::STA, ADDRMODE::ABY, 3, 5 },
		{ 0x9A, INSTRUCTION::TXS, ADDRMODE::IMP, 1, 2 },
		{ 0x9B, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x9C, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x9D, INSTRUCTION::STA, ADDRMODE::ABX, 3, 5 },
		{ 0x9E, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0x9F, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xA0, INSTRUCTION::LDY, ADDRMODE::IMM, 2, 2 },
		{ 0xA1, INSTRUCTION::LDA, ADDRMODE::INX, 2, 6 },
		{ 0xA2, INSTRUCTION::LDX, ADDRMODE::IMM, 2, 2 },
		{ 0xa3, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xA4, INSTRUCTION::LDY, ADDRMODE::ZPG, 2, 3 },
		{ 0xA5, INSTRUCTION::LDA, ADDRMODE::ZPG, 2, 3 },
		{ 0xA6, INSTRUCTION::LDX, ADDRMODE::ZPG, 2, 3 },
		{ 0xa7, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xA8, INSTRUCTION::TAY, ADDRMODE::IMP, 1, 2 },
		{ 0xA9, INSTRUCTION::LDA, ADDRMODE::IMM, 2, 2 },
		{ 0xAA, INSTRUCTION::TAX, ADDRMODE::IMP, 1, 2 },
		{ 0xAB, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xAC, INSTRUCTION::LDY, ADDRMODE::ABS, 3, 4 },
		{ 0xAD, INSTRUCTION::LDA, ADDRMODE::ABS, 3, 4 },
		{ 0xAE, INSTRUCTION::LDX, ADDRMODE::ABS, 3, 4 },
		{ 0xAF, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xB0, INSTRUCTION::BCS, ADDRMODE::REL, 2, 2 },
		{ 0xB1, INSTRUCTION::LDA, ADDRMODE::INY, 2, 5 },
		{ 0xB2, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xB3, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xB4, INSTRUCTION::LDY, ADDRMODE::ZPX, 2, 4 },
		{ 0xB5, INSTRUCTION::LDA, ADDRMODE::ZPX, 2, 4 },
		{ 0xB6, INSTRUCTION::LDX, ADDRMODE::ZPY, 2, 4 },
		{ 0xB7, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xB8, INSTRUCTION::CLV, ADDRMODE::IMP, 1, 2 },
		{ 0xB9, INSTRUCTION::LDA, ADDRMODE::ABY, 3, 4 },
		{ 0xBA, INSTRUCTION::TSX, ADDRMODE::IMP, 1, 2 },
		{ 0xBB, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xBC, INSTRUCTION::LDY, ADDRMODE::ABX, 3, 4 },
		{ 0xBD, INSTRUCTION::LDA, ADDRMODE::ABX, 3, 4 },
		{ 0xBE, INSTRUCTION::LDX, ADDRMODE::ABY, 3, 4 },
		{ 0xBF, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xC0, INSTRUCTION::CPY, ADDRMODE::IMM, 2, 2 },
		{ 0xC1, INSTRUCTION::CMP, ADDRMODE::INX, 2, 6 },
		{ 0xC2, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xC3, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xC4, INSTRUCTION::CPY, ADDRMODE::ZPG, 2, 3 },
		{ 0xC5, INSTRUCTION::CMP, ADDRMODE::ZPG, 2, 3 },
		{ 0xC6, INSTRUCTION::DEC, ADDRMODE::ZPG, 2, 5 },
		{ 0xC7, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xC8, INSTRUCTION::INY, ADDRMODE::IMP, 1, 2 },
		{ 0xC9, INSTRUCTION::CMP, ADDRMODE::IMM, 2, 2 },
		{ 0xCA, INSTRUCTION::DEX, ADDRMODE::IMP, 1, 2 },
		{ 0xCB, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xCC, INSTRUCTION::CPY, ADDRMODE::ABS, 3, 4 },
		{ 0xCD, INSTRUCTION::CMP, ADDRMODE::ABS, 3, 4 },
		{ 0xCE, INSTRUCTION::DEC, ADDRMODE::ABS, 3, 6 },
		{ 0xCF, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xD0, INSTRUCTION::BNE, ADDRMODE::REL, 2, 2 },
		{ 0xD1, INSTRUCTION::CMP, ADDRMODE::INY, 2, 5 },
		{ 0xD2, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xD3, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xD4, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xD5, INSTRUCTION::CMP, ADDRMODE::ZPX, 2, 4 },
		{ 0xD6, INSTRUCTION::DEC, ADDRMODE::ZPX, 2, 6 },
		{ 0xD7, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xD8, INSTRUCTION::CLD, ADDRMODE::IMP, 1, 2 },
		{ 0xD9, INSTRUCTION::CMP, ADDRMODE::ABY, 3, 4 },
		{ 0xDA, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xDB, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xDC, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xDD, INSTRUCTION::CMP, ADDRMODE::ABX, 3, 4 },
		{ 0xDE, INSTRUCTION::DEC, ADDRMODE::ABX, 3, 7 },
		{ 0xDF, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xE0, INSTRUCTION::CPX, ADDRMODE::IMM, 2, 2 },
		{ 0xE1, INSTRUCTION::SBC, ADDRMODE::INX, 2, 6 },
		{ 0xE2, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xE3, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xE4, INSTRUCTION::CPX, ADDRMODE::ZPG, 2, 3 },
		{ 0xE5, INSTRUCTION::SBC, ADDRMODE::ZPG, 2, 3 },
		{ 0xE6, INSTRUCTION::INC, ADDRMODE::ZPG, 2, 5 },
		{ 0xE7, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xE8, INSTRUCTION::INX, ADDRMODE::IMP, 1, 2 },
		{ 0xE9, INSTRUCTION::SBC, ADDRMODE::IMM, 2, 2 },
		{ 0xEA, INSTRUCTION::NOP, ADDRMODE::IMP, 1, 2 },
		{ 0xEB, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xEC, INSTRUCTION::CPX, ADDRMODE::ABS, 3, 4 },
		{ 0xED, INSTRUCTION::SBC, ADDRMODE::ABS, 3, 4 },
		{ 0xEE, INSTRUCTION::INC, ADDRMODE::ABS, 3, 6 },
		{ 0xEF, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xF0, INSTRUCTION::BEQ, ADDRMODE::REL, 2, 2 },
		{ 0xF1, INSTRUCTION::SBC, ADDRMODE::INY, 2, 5 },
		{ 0xF2, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xF3, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xF4, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xF5, INSTRUCTION::SBC, ADDRMODE::ZPX, 2, 4 },
		{ 0xF6, INSTRUCTION::INC, ADDRMODE::ZPX, 2, 6 },
		{ 0xF7, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xF8, INSTRUCTION::SED, ADDRMODE::IMP, 1, 2 },
		{ 0xF9, INSTRUCTION::SBC, ADDRMODE::ABY, 3, 4 },
		{ 0xFA, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xFB, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xFC, INSTRUCTION::ILL, ADDRMODE::ILL, 0, 0 },
		{ 0xFD, INSTRUCTION::SBC, ADDRMODE::ABX, 3, 4 },
		{ 0xFE, INSTRUCTION::INC, ADDRMODE::ABX, 3, 7 },
	};
}

void CPU6502::fetch_fullinstruction(const std::uint8_t &op_code)
{
	// searches for the instruction in the opcode matrix by its hexcode.
	this->current_instruction = instr_matrix[op_code];
}

void CPU6502::decode_instruction(const std::uint16_t &operand)
{
	switch(current_instruction.addr_mode) {
	case ADDRMODE::ABS:
		exec_instruction(operand);
		break;
	case ADDRMODE::ABX:
		exec_instruction(operand + x);
		break;
	case ADDRMODE::ABY:
		exec_instruction(operand + y);
		break;
	case ADDRMODE::ACC:
		exec_instruction(operand); // fix
		break;
	case ADDRMODE::IMM:
		exec_instruction(operand);
		break;
	case ADDRMODE::IMP:
		exec_instruction(operand);
		break;
	case ADDRMODE::IND:

		break;
	case ADDRMODE::INX:

		break;
	case ADDRMODE::INY:

		break;
	case ADDRMODE::REL:

		break;
	case ADDRMODE::ZPG:

		break;
	case ADDRMODE::ZPX:

		break;
	case ADDRMODE::ZPY:

		break;

	case ADDRMODE::ILL:

		break;
	}
}

void CPU6502::exec_instruction(const std::uint16_t &address)
{
	spdlog::debug("opcode=0x{:02x}, operand=0x{:04x}", current_instruction.op_code, address);
	spdlog::debug("pc=0x{:04x}, acc=0x{:02x}, x=0x{:02x}, y=0x{:02x}, sp=0x{:02x}, sr=0b{:08b}", pc, acc, x, y, sp, sr);

	switch(current_instruction.instr) {
	case INSTRUCTION::ADC:
		if (acc > 0 && read(address) > std::numeric_limits<std::uint8_t>::max() - acc - (sr & (1 << 0))) {
			//sr |= 0b10000001; // definitely wrong; must study more.
		}

		acc += read(address) + (sr & (1 << 0));

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::AND:
		acc &= read(address);

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::ASL:

		break;
	case INSTRUCTION::BCC:
		(sr & 0b00000001) == 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BCS:
		(sr & 0b00000001) != 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BEQ:
		(sr & 0b00000010) != 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BIT:

		break;
	case INSTRUCTION::BMI:
		(sr & 0b1000000) != 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BNE:
		(sr & 0b00000010) == 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BPL:
		(sr & 0b10000000) == 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BRK:
		stack.push(pc);
		stack.push(sr);
		pc = 0xFFFE;
		sr |= 0b00010000;

		break;
	case INSTRUCTION::BVC:
		(sr & 0b01000000) == 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::BVS:
		(sr & 0b01000000) != 0
			? pc += address
			: pc++;

		break;
	case INSTRUCTION::CLC:
		sr &= 0b11111110;
		pc++;

		break;
	case INSTRUCTION::CLD:
		sr &= 0b11110111;
		pc++;

		break;
	case INSTRUCTION::CLI:
		sr &= 0b11111011;
		pc++;

		break;
	case INSTRUCTION::CLV:
		sr &= 0b10111111;
		pc++;

		break;
	case INSTRUCTION::CMP:
		if (acc >= read(address)) {
			sr |= 0b00000001;
		}

		if (acc == read(address)) {
			sr |= 0b00000010;
		}

		if (((acc - read(address)) >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::CPX:
		if (acc >= read(address)) {
			sr |= 0b00000001;
		}

		if (acc == read(address)) {
			sr |= 0b00000010;
		}

		if (((x - read(address))) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::CPY:
		if (acc >= read(address)) {
			sr |= 0b00000001;
		}

		if (acc == read(address)) {
			sr |= 0b00000010;
		}

		if (((y - read(address))) & 1) {
			sr |= 0b10000000;
		}

		break;
	case INSTRUCTION::DEC:
		write(address, read(address) - 1);

		if (read(address) == 0) {
			sr |= 0b00000010;
		}

		if ((read(address) >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::DEX:
		x--;

		if (x == 0) {
			sr |= 0b00000010;
		}

		if ((x >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::DEY:
		y--;

		if (y == 0) {
			sr |= 0b00000010;
		}

		if ((y >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::EOR:
		acc ^= read(address);

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INC:
		write(address, read(address) + 1);

		if (read(address) == 0) {
			sr |= 0b00000010;
		}

		if ((read(address) >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INX:
		x++;

		if (x == 0) {
			sr |= 0b00000010;
		}

		if ((x >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INY:
		y++;

		if (y == 0) {
			sr |= 0b00000010;
		}

		if ((y >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::JMP:
		pc = address;

		break;
	case INSTRUCTION::JSR:
		stack.push(pc - 1);
		pc = address;

		break;
	case INSTRUCTION::LDA:
		acc = read(address);

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LDX:
		x = read(address);

		if (x == 0) {
			sr |= 0b00000010;
		}

		if ((x >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LDY:
		y = read(address);

		if (y == 0) {
			sr |= 0b00000010;
		}

		if ((y >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LSR:

		break;
	case INSTRUCTION::NOP:
		pc++;

		break;
	case INSTRUCTION::ORA:
		acc |= read(address);

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		break;
	case INSTRUCTION::PHA:
		stack.push(acc);
		pc++;

		break;
	case INSTRUCTION::PHP:
		stack.push(sr);
		pc++;

		break;
	case INSTRUCTION::PLA:
		acc = stack.top();
		stack.pop();

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::PLP:
		sr = stack.top();
		stack.pop();

		pc++;

		break;
	case INSTRUCTION::ROL:

		break;
	case INSTRUCTION::ROR:

		break;
	case INSTRUCTION::RTI:

		break;
	case INSTRUCTION::RTS:
		pc = stack.top();
		stack.pop();

		break;
	case INSTRUCTION::SBC:

		break;
	case INSTRUCTION::SEC:
		sr |= 0b00000001;
		pc++;

		break;
	case INSTRUCTION::SED:
		sr |= 0b00001000;
		pc++;

		break;
	case INSTRUCTION::SEI:
		sr |= 0b00000100;
		pc++;

		break;
	case INSTRUCTION::STA:
		write(address, acc);
		pc++;

		break;
	case INSTRUCTION::STX:
		write(address, x);
		pc++;

		break;
	case INSTRUCTION::STY:
		write(address, y);
		pc++;

		break;
	case INSTRUCTION::TAX:
		x = acc;

		if (x == 0) {
			sr |= 0b00000010;
		}

		if ((x >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TAY:
		y = acc;

		if (y == 0) {
			sr |= 0b00000010;
		}

		if ((y >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TSX:
		x = sp;

		if (x == 0) {
			sr |= 0b00000010;
		}

		if ((x >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TXA:
		acc = x;

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TXS:
		sp = x;
		pc++;

		break;
	case INSTRUCTION::TYA:
		acc = y;

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;

	default:

		break;
	}
}

void CPU6502::clear()
{
	acc = x = y = sp = sr = 0x00;
	pc = 0x0000;
	stack = std::stack<std::uint8_t>();
	spdlog::debug("Cleared CPU's memory and registers.");
}

void CPU6502::nes_init_regs()
{
	acc = x = y = sp = sr = 0x00;
	pc = 0x0000;
	spdlog::debug("CPU registers and memory initialised.");
}

void CPU6502::clock_cycle()
{
	if (cycles == 0) {
		opcode = read(pc);
		pc++;

		cycles = instr_matrix[opcode].cycles;
	}

	cycles--;
}

std::uint8_t CPU6502::read(const std::uint16_t &addr)
{
	return bus->read(addr);
}

void CPU6502::write(const std::uint16_t &addr, const std::uint8_t &data)
{
	bus->write(addr, data);
}

void CPU6502::set_bus(const std::shared_ptr<Bus> &bus)
{
	this->bus = bus;
}

CPU6502::~CPU6502()
{

}
