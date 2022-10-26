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
		pc = 0xFFFEu;
		sr |= 0b00010000u;
		break;

	case 0x10: // BPL rel
		if (((sr >> 7) & 1) == 0b1) {

		}
		break;

	case 0x20: // JSR abs

		break;

	case 0x30: // BMI rel

		break;

	case 0x40: // RTI

		break;

	case 0x50: // BVC rel

		break;

	case 0x60: // RTS

		break;

	case 0x70: // BVS rel

		break;

	case 0x90: // BCC rel

		break;

	case 0xA0: // LDY #
		if (iy == 0x00u) {
			sr |= 0b00000010u;
		}

		if ((iy >> 7) & 1) {
			sr |= 0b10000000u;
		}

		break;

	case 0xB0: // BCS rel

		break;

	case 0xC0: // CPY #

		break;

	case 0xD0: // BNE rel
		if (((sr >> 1) & 1) == 0b0) {
			pc += 2;
		}
		break;

	case 0xE0: // CPX #

		break;

	case 0xF0: // BEQ rel
		if (((sr >> 1) & 1) == 0b1) {
			pc += 2;
		}
		break;

	default:
		break;
	}
}

RP2A03::~RP2A03()
{

}
