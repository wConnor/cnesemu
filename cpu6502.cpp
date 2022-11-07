#include "cpu6502.hpp"

CPU6502::CPU6502()
{

}

std::uint8_t CPU6502::fetch_operand_length(const std::uint8_t &op_code)
{
	// searches for the instruction in the opcode matrix by its hexcode.
	this->current_instruction = *std::find_if(instr_matrix.begin(),
											  instr_matrix.end(),
											  [&op_code]
											  (const INSTRUCTIONINFO &instr_info) -> bool { return instr_info.op_code == op_code; });

	// instruction found in matrix
	switch (current_instruction.addr_mode) {
	case ADDRMODE::ABS:
	case ADDRMODE::ABX:
	case ADDRMODE::ABY:
		return 3;

	case ADDRMODE::ACC:
		return 0;

	case ADDRMODE::IMM:
		return 2;

	case ADDRMODE::IMP:
		return 1;

	case ADDRMODE::IND:
		return 3;

	case ADDRMODE::INX:
	case ADDRMODE::INY:
		return 2;

	case ADDRMODE::REL:
		return 2;

	case ADDRMODE::ZPG:
	case ADDRMODE::ZPX:
	case ADDRMODE::ZPY:
		return 2;
	}

	return 0;
}

void CPU6502::decode_instruction(const std::uint16_t &operand)
{
	switch(current_instruction.addr_mode) {
	case ADDRMODE::ABS:
		exec_instruction(operand);
		break;
	case ADDRMODE::ABX:
		exec_instruction(operand + ix);
		break;
	case ADDRMODE::ABY:
		exec_instruction(operand + iy);
		break;
	case ADDRMODE::ACC:
		exec_instruction(acc); // fix
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
	}

}

void CPU6502::exec_instruction(const std::uint16_t &address)
{
	spdlog::debug("opcode=0x{0:2x}, operand=0x{0:4x}", current_instruction.op_code, address);
	spdlog::debug("pc=0x{0:4x}, acc=0x{0:2x}, ix=0x{0:2x}, iy=0x{0:2x}, sp=0x{0:2x}, sr=0b{0:8b}", pc, acc, ix, iy, sp, sr);

	switch(current_instruction.instr) {
	case INSTRUCTION::ADC:
		if (acc > 0 && (*mem)[address] > std::numeric_limits<std::uint8_t>::max() - acc - (sr & (1 << 0))) {
			//sr |= 0b10000001; // definitely wrong; must study more.
		}

		acc += (*mem)[address] + (sr & (1 << 0));

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::AND:
		acc &= (*mem)[address];

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
		if (acc >= (*mem)[address]) {
			sr |= 0b00000001;
		}

		if (acc == (*mem)[address]) {
			sr |= 0b00000010;
		}

		if (((acc - (*mem)[address]) >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::CPX:
		if (acc >= (*mem)[address]) {
			sr |= 0b00000001;
		}

		if (acc == (*mem)[address]) {
			sr |= 0b00000010;
		}

		if (((ix - (*mem)[address])) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::CPY:
		if (acc >= (*mem)[address]) {
			sr |= 0b00000001;
		}

		if (acc == (*mem)[address]) {
			sr |= 0b00000010;
		}

		if (((iy - (*mem)[address])) & 1) {
			sr |= 0b10000000;
		}

		break;
	case INSTRUCTION::DEC:
		(*mem)[address]--;

		if ((*mem)[address] == 0) {
			sr |= 0b00000010;
		}

		if (((*mem)[address] >> 7) & 1) {
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
		acc ^= (*mem)[address];

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::INC:
		(*mem)[address]++;

		if ((*mem)[address] == 0) {
			sr |= 0b00000010;
		}

		if (((*mem)[address] >> 7) & 1) {
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
		acc = (*mem)[address];

		if (acc == 0) {
			sr |= 0b00000010;
		}

		if ((acc >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LDX:
		ix = (*mem)[address];

		if (ix == 0) {
			sr |= 0b00000010;
		}

		if ((ix >> 7) & 1) {
			sr |= 0b10000000;
		}

		pc++;

		break;
	case INSTRUCTION::LDY:
		iy = (*mem)[address];

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
		acc |= (*mem)[address];

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
		(*mem)[address] = acc;
		pc++;

		break;
	case INSTRUCTION::STX:
		(*mem)[address] = ix;
		pc++;

		break;
	case INSTRUCTION::STY:
		(*mem)[address] = iy;
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
	std::fill(mem->begin(), mem->end(), 0x00);
	stack = std::stack<std::uint8_t>();
	spdlog::debug("Cleared CPU's memory and registers.");
}

void CPU6502::nes_init_regs()
{
	acc = ix = iy = sp = sr = 0x00;
	pc = 0x0000;
	std::fill(mem->begin(), mem->end(), 0x00);
	spdlog::debug("CPU registers and memory initialised.");
}

CPU6502::~CPU6502()
{

}

void CPU6502::set_mem(const std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> &mem) {
	this->mem = mem;
}
