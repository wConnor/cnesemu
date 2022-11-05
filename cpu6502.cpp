#include "cpu6502.hpp"

CPU6502::CPU6502()
{

}

CPU6502::INSTRUCTIONINFO CPU6502::fetch_instruction(const std::uint8_t &instruction)
{
	// searches for the instruction in the opcode matrix by its hexcode.
	auto full_instruction = std::find_if(instr_matrix.begin(),
										 instr_matrix.end(),
										 [&instruction]
										 (const INSTRUCTIONINFO &instr_info) -> bool { return instruction == instr_info.op_code; });

	// instruction found in matrix
	return *full_instruction;
}

void CPU6502::decode_instruction(const INSTRUCTIONINFO &full_instruction)
{

}

void CPU6502::exec_instruction(const INSTRUCTION &instruction, const std::uint16_t &address)
{
	switch(instruction) {
	case INSTRUCTION::ADC:
		if (acc > 0 && mem[address] > std::numeric_limits<std::uint8_t>::max() - acc - (sr & (1 << 0))) {
			//sr |= 0b10000001; // definitely wrong; must study more.
		}

		acc += mem[address] + (sr & (1 << 0));

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::AND:
		acc &= mem[address];

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
		if (acc >= mem[address]) {
			sr |= 0b00000001;
		}

		if (acc == mem[address]) {
			sr |= 0b00000010;
		}

		if (((acc - mem[address]) >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::CPX:
		if (acc >= mem[address]) {
			sr |= 0b00000001;
		}

		if (acc == mem[address]) {
			sr |= 0b00000010;
		}

		if (((ix - mem[address])) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::CPY:
		if (acc >= mem[address]) {
			sr |= 0b00000001;
		}

		if (acc == mem[address]) {
			sr |= 0b00000010;
		}

		if (((iy - mem[address])) & 1) {
			sr |= 0b10000000;
		}

		break;
	case INSTRUCTION::DEC:
		mem[address]--;

		if (mem[address] == 0) {
			sr |= 0b00000010;
		}

		if ((mem[address] >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::DEX:
		ix--;

		if (ix == 0) {
			sr |= 0b00000010;
		}

		if ((ix >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::DEY:
		iy--;

		if (iy == 0) {
			sr |= 0b00000010;
		}

		if ((iy >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::EOR:
		acc ^= mem[address];

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INC:
		mem[address]++;

		if (mem[address] == 0) {
			sr |= 0b00000010;
		}

		if ((mem[address] >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INX:
		ix++;

		if (ix == 0) {
			sr |= 0b00000010;
		}

		if ((ix >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INY:
		iy++;

		if (iy == 0) {
			sr |= 0b00000010;
		}

		if ((iy >> 7) & 1) {
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
		acc = mem[address];

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LDX:
		ix = mem[address];

		if (ix == 0) {
			sr |= 0b00000010;
		}

		if ((ix >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LDY:
		iy = mem[address];

		if (iy == 0) {
			sr |= 0b00000010;
		}

		if ((iy >> 7) & 1) {
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
		acc |= mem[address];

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
		mem[address] = acc;
		pc++;

		break;
	case INSTRUCTION::STX:
		mem[address] = ix;
		pc++;

		break;
	case INSTRUCTION::STY:
		mem[address] = iy;
		pc++;

		break;
	case INSTRUCTION::TAX:
		ix = acc;

		if (ix == 0) {
			sr |= 0b00000010;
		}

		if ((ix >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TAY:
		iy = acc;

		if (iy == 0) {
			sr |= 0b00000010;
		}

		if ((iy >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TSX:
		ix = sp;

		if (ix == 0) {
			sr |= 0b00000010;
		}

		if ((ix >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TXA:
		acc = ix;

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::TXS:
		sp = ix;
		pc++;

		break;
	case INSTRUCTION::TYA:
		acc = iy;

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
	acc = ix = iy = sp = sr = 0x00;
	pc = 0x0000;
	std::fill(mem.begin(), mem.end(), 0x00);
	stack = std::stack<std::uint8_t>();
	spdlog::debug("Cleared CPU's memory and registers.");
}

CPU6502::~CPU6502()
{

}
