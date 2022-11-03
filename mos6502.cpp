#include "mos6502.hpp"

MOS6502::MOS6502()
{

}

MOS6502::INSTRUCTIONINFO MOS6502::fetch_instruction(const std::uint8_t &instruction)
{
	// searches for the instruction in the opcode matrix by its hexcode.
	auto full_instruction = std::find_if(instr_matrix.begin(),
							instr_matrix.end(),
							[&instruction]
							(const INSTRUCTIONINFO &instr_info) -> bool { return instruction == instr_info.op_code; });

	// instruction found in matrix
	return *full_instruction;
}

void MOS6502::decode_instruction(const INSTRUCTIONINFO &full_instruction)
{

}

void MOS6502::exec_instruction(const std::uint8_t &instruction)
{

}

MOS6502::~MOS6502()
{

}
