#include "rp2a03.hpp"

RP2A03::RP2A03()
{

}

RP2A03::INSTRUCTIONINFO RP2A03::fetch_instruction(const std::uint8_t &instruction)
{
	// searches for the instruction in the opcode matrix by its hexcode.
	auto full_instruction = std::find_if(instr_matrix.begin(),
							instr_matrix.end(),
							[&instruction]
							(const INSTRUCTIONINFO &instr_info) -> bool { return instruction == instr_info.op_code; });

	// instruction found in matrix
	return *full_instruction;
}

void RP2A03::decode_instruction(const INSTRUCTIONINFO &full_instruction)
{

}

void RP2A03::exec_instruction(const std::uint8_t &instruction)
{

}

RP2A03::~RP2A03()
{

}
