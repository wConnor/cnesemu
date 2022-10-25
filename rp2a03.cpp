#include "rp2a03.hpp"

RP2A03::RP2A03()
{

}

void RP2A03::exec_instruction(std::uint8_t &opcode)
{
	switch (opcode) {

	case 0x00: // BRK
		stack.push(pc);
		stack.push(sr);
		pc = 0xFFFE;
		sr = sr | 0b00010000u;
		break;

	case 0x10: // BPL rel
		if ((sr >> 7) & 1) {

		}
		break;

	default:
		break;
	}
}

RP2A03::~RP2A03()
{

}
