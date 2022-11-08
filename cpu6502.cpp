#include "cpu6502.hpp"

CPU6502::CPU6502()
{
	this->addr_mode_map = {
		{ "ABS", std::bind(&CPU6502::ABS, this) },
		{ "ABX", std::bind(&CPU6502::ABX, this) },
		{ "ABY", std::bind(&CPU6502::ABY, this) },
		{ "ACC", std::bind(&CPU6502::ACC, this) },
		{ "IND", std::bind(&CPU6502::IND, this) },
		{ "IZX", std::bind(&CPU6502::IZX, this) },
		{ "IZY", std::bind(&CPU6502::IZY, this) },
		{ "IMP", std::bind(&CPU6502::IMP, this) },
		{ "IMM", std::bind(&CPU6502::IMM, this) },
		{ "ZPG", std::bind(&CPU6502::ZPG, this) },
		{ "ZPX", std::bind(&CPU6502::ZPX, this) },
		{ "ZPY", std::bind(&CPU6502::ZPY, this) },
		{ "REL", std::bind(&CPU6502::REL, this) },

		{ "ILL", std::bind(&CPU6502::ILL, this) },
	};

	this->instr_map = {
		{ "ADC", std::bind(&CPU6502::ADC, this) },
		{ "AND", std::bind(&CPU6502::AND, this) },
		{ "ASL", std::bind(&CPU6502::ASL, this) },
		{ "BCC", std::bind(&CPU6502::BCC, this) },
		{ "BEQ", std::bind(&CPU6502::BEQ, this) },
		{ "BIT", std::bind(&CPU6502::BIT, this) },
		{ "BMI", std::bind(&CPU6502::BMI, this) },
		{ "BNE", std::bind(&CPU6502::BNE, this) },
		{ "BPL", std::bind(&CPU6502::BPL, this) },
		{ "BRK", std::bind(&CPU6502::BRK, this) },
		{ "BVC", std::bind(&CPU6502::BVC, this) },
		{ "BVS", std::bind(&CPU6502::BVS, this) },
		{ "CLC", std::bind(&CPU6502::CLC, this) },
		{ "CLD", std::bind(&CPU6502::CLD, this) },
		{ "CLI", std::bind(&CPU6502::CLI, this) },
		{ "CLV", std::bind(&CPU6502::CLV, this) },
		{ "CMP", std::bind(&CPU6502::CMP, this) },
		{ "CPX", std::bind(&CPU6502::CPX, this) },
		{ "CPY", std::bind(&CPU6502::CPY, this) },
		{ "DEC", std::bind(&CPU6502::DEC, this) },
		{ "DEX", std::bind(&CPU6502::DEX, this) },
		{ "DEY", std::bind(&CPU6502::DEY, this) },
		{ "EOR", std::bind(&CPU6502::EOR, this) },
		{ "INC", std::bind(&CPU6502::INC, this) },
		{ "INX", std::bind(&CPU6502::INX, this) },
		{ "INY", std::bind(&CPU6502::INY, this) },
		{ "JMP", std::bind(&CPU6502::JMP, this) },
		{ "JSR", std::bind(&CPU6502::JSR, this) },
		{ "LDA", std::bind(&CPU6502::LDA, this) },
		{ "LDX", std::bind(&CPU6502::LDX, this) },
		{ "LDY", std::bind(&CPU6502::LDY, this) },
		{ "LSR", std::bind(&CPU6502::LSR, this) },
		{ "NOP", std::bind(&CPU6502::NOP, this) },
		{ "ORA", std::bind(&CPU6502::ORA, this) },
		{ "PHA", std::bind(&CPU6502::PHA, this) },
		{ "PHP", std::bind(&CPU6502::PHP, this) },
		{ "PLA", std::bind(&CPU6502::PLA, this) },
		{ "PLP", std::bind(&CPU6502::PLP, this) },
		{ "ROL", std::bind(&CPU6502::ROL, this) },
		{ "ROR", std::bind(&CPU6502::ROR, this) },
		{ "RTI", std::bind(&CPU6502::RTI, this) },
		{ "RTS", std::bind(&CPU6502::RTS, this) },
		{ "SBC", std::bind(&CPU6502::SBC, this) },
		{ "SEC", std::bind(&CPU6502::SEC, this) },
		{ "SED", std::bind(&CPU6502::SED, this) },
		{ "SEI", std::bind(&CPU6502::SEI, this) },
		{ "STA", std::bind(&CPU6502::STA, this) },
		{ "STX", std::bind(&CPU6502::STX, this) },
		{ "STY", std::bind(&CPU6502::STY, this) },
		{ "TAX", std::bind(&CPU6502::TAX, this) },
		{ "TAY", std::bind(&CPU6502::TAY, this) },
		{ "TSX", std::bind(&CPU6502::TSX, this) },
		{ "TXA", std::bind(&CPU6502::TXA, this) },
		{ "TXS", std::bind(&CPU6502::TXS, this) },
		{ "TYA", std::bind(&CPU6502::TYA, this) },

		{ "ILL", std::bind(&CPU6502::ILL, this) },
	};

	// instruction matrix from https://www.masswerk.at/6502/6502_instruction_set.html
	this->instr_matrix = {
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
		{ 0xa7, "ILL", "ILL", 0, 0 },
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
	};
}

void CPU6502::fetch_fullinstruction(const std::uint8_t &op_code)
{
	// searches for the instruction in the opcode matrix by its hexcode.
	this->current_instruction = instr_matrix[op_code];
}

void CPU6502::decode_instruction(const std::uint16_t &operand)
{

}

void CPU6502::exec_instruction(const std::uint16_t &address)
{
	spdlog::debug("opcode=0x{:02x}, operand=0x{:04x}", current_instruction.opcode, address);
	spdlog::debug("pc=0x{:04x}, acc=0x{:02x}, x=0x{:02x}, y=0x{:02x}, sp=0x{:02x}, sr=0b{:08b}", pc, acc, x, y, sp, sr);
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

/* **** ADDRESSING MODES **** */
void CPU6502::ABS()
{
	addr_abs = (read(pc) << 8) | read(pc + 1);
	pc += 2;

}

void CPU6502::ABX()
{

}

void CPU6502::ABY()
{

}

void CPU6502::ACC()
{

}

void CPU6502::IND()
{

}

void CPU6502::IZX()
{

}

void CPU6502::IZY()
{

}

void CPU6502::IMP()
{
	fetched = acc;

}

void CPU6502::IMM()
{

}

void CPU6502::ZPG()
{

}

void CPU6502::ZPX()
{

}

void CPU6502::ZPY()
{

}

void CPU6502::REL()
{
	addr_rel = read(pc);

}

/* **** INSTRUCTIONS **** */
void CPU6502::ADC()
{
	fetch();
	if (acc > 0 && fetched > std::numeric_limits<std::uint8_t>::max() - acc - (sr & (1 << 0))) {
		//sr |= 0b10000001; // definitely wrong; must study more.
	}

	acc += fetched + (sr & (1 << 0));

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::AND()
{
	fetch();
	acc &= fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::ASL()
{

}

void CPU6502::BCC()
{
	(sr & 0b00000001) == 0
		? pc += addr_rel
		: pc++;

}

void CPU6502::BCS()
{
	(sr & 0b00000001) != 0
		? pc += addr_rel
		: pc++;

}

void CPU6502::BEQ()
{
	(sr & 0b00000010) != 0
		? pc += addr_rel
		: pc++;

}

void CPU6502::BIT()
{

}

void CPU6502::BMI()
{
	(sr & 0b1000000) != 0
		? pc += addr_rel
		: pc++;
}

void CPU6502::BNE()
{
	(sr & 0b00000010) == 0
		? pc += addr_rel
		: pc++;

}

void CPU6502::BPL()
{
	(sr & 0b10000000) == 0
		? pc += addr_rel
		: pc++;

}

void CPU6502::BRK()
{
	stack.push(pc);
	stack.push(sr);
	pc = 0xFFFE;
	sr |= 0b00010000;
}

void CPU6502::BVC()
{
	(sr & 0b01000000) == 0
		? pc += addr_rel
		: pc++;

}

void CPU6502::BVS()
{
	(sr & 0b01000000) != 0
		? pc += addr_rel
		: pc++;
}

void CPU6502::CLC()
{
	sr &= 0b11111110;
	pc++;

}

void CPU6502::CLD()
{
	sr &= 0b11110111;
	pc++;

}

void CPU6502::CLI()
{
	sr &= 0b11111011;
	pc++;

}

void CPU6502::CLV()
{
	sr &= 0b10111111;
	pc++;
}

void CPU6502::CMP()
{
	// if (acc >= read(address)) {
	//	sr |= 0b00000001;
	// }

	// if (acc == read(address)) {
	//	sr |= 0b00000010;
	// }

	// if (((acc - read(address)) >> 7) & 1) {
	//	sr |= 0b10000000;
	// }

	// pc++;

}

void CPU6502::CPX()
{
	// if (acc >= read(address)) {
	//	sr |= 0b00000001;
	// }

	// if (acc == read(address)) {
	//	sr |= 0b00000010;
	// }

	// if (((x - read(address))) & 1) {
	//	sr |= 0b10000000;
	// }

	// pc++;

}

void CPU6502::CPY()
{
	fetch();
	if (acc >= fetched) {
		sr |= 0b00000001;
	}

	if (acc == fetched) {
		sr |= 0b00000010;
	}

	if (((y - fetched)) & 1) {
		sr |= 0b10000000;
	}

}

void CPU6502::DEC()
{
	fetch();
	write(addr_abs, (fetched - 1) & 0x00FF);

	if (fetched - 1 == 0) {
		sr |= 0b00000010;
	}

	if (((fetched - 1) >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::DEX()
{
	x--;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::DEY()
{
	y--;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::EOR()
{
	fetch();
	acc ^= fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::INC()
{
	fetch();
	write(addr_abs, (fetched + 1) & 0x00FF);

	if (fetched + 1 == 0) {
		sr |= 0b00000010;
	}

	if (((fetched + 1) >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::INX()
{
	x++;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::INY()
{
	y++;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::JMP()
{
	pc = addr_abs;
}

void CPU6502::JSR()
{
	stack.push(pc - 1);
	pc = addr_abs;

}

void CPU6502::LDA()
{
	fetch();
	acc = fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::LDX()
{
	fetch();
	x = fetched;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::LDY()
{
	fetch();
	y = fetched;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::LSR()
{

}

void CPU6502::NOP()
{
	pc++;
}

void CPU6502::ORA()
{
	fetch();
	acc |= fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

}

void CPU6502::PHA()
{
	stack.push(acc);
	pc++;

}

void CPU6502::PHP()
{
	stack.push(sr);
	pc++;

}

void CPU6502::PLA()
{
	acc = stack.top();
	stack.pop();

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::PLP()
{
	sr = stack.top();
	stack.pop();

	pc++;

}

void CPU6502::ROL()
{

}

void CPU6502::ROR()
{

}

void CPU6502::RTI()
{

}

void CPU6502::RTS()
{
	pc = stack.top();
	stack.pop();

}

void CPU6502::SBC()
{

}

void CPU6502::SEC()
{
	sr |= 0b00000001;
	pc++;

}

void CPU6502::SED()
{
	sr |= 0b00001000;
	pc++;

}

void CPU6502::SEI()
{
	sr |= 0b00000100;
	pc++;
}

void CPU6502::STA()
{
	write(addr_abs, acc);
	pc++;

}

void CPU6502::STX()
{
	write(addr_abs, x);
	pc++;

}

void CPU6502::STY()
{
	write(addr_abs, y);
	pc++;

}

void CPU6502::TAX()
{
	x = acc;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;


}

void CPU6502::TAY()
{
	y = acc;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::TSX()
{
	x = sp;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::TXA()
{
	acc = x;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::TXS()
{
	sp = x;
	pc++;

}

void CPU6502::TYA()
{
	acc = y;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

}

void CPU6502::ILL()
{

}

std::uint8_t CPU6502::fetch()
{
	if (instr_matrix[opcode].addr_mode != "IMP") {
		fetched = read(addr_abs);
	}

	return fetched;
}

CPU6502::~CPU6502()
{

}
