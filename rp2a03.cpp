#include "rp2a03.hpp"

RP2A03::RP2A03()
{

}

void RP2A03::exec_instruction(std::uint16_t &instruction)
{
	switch (instruction & 0xFF00u) {
	case 0x0000: // BRK
		stack.push(pc);
		stack.push(sr);
		pc = 0xFFFEu;
		sr |= 0b00010000u;
		break;

	case 0x1000: // BPL rel
		if (((sr >> 7) & 1) == 0b0) {
			pc += (instruction & 0x00FFu);
		}
		break;

	case 0x2000: // JSR abs

		break;

	case 0x3000: // BMI rel
		if (((sr >> 7) & 1) == 0b1) {
			pc += (instruction & 0x00FFu);
		}
			
		break;

	case 0x4000: // RTI

		break;

	case 0x5000: // BVC rel

		break;

	case 0x6000: // RTS

		break;

	case 0x7000: // BVS rel

		break;

	case 0x9000: // BCC rel

		break;

	case 0xA000: // LDY #
		iy = mem[(instruction & 0x00FFu)];

		if (iy == 0x00u) {
			sr |= 0b00000010u;
		}

		if ((iy >> 7) & 1) {
			sr |= 0b10000000u;
		}

		break;

	case 0xB000: // BCS rel

		break;

	case 0xC000: // CPY #
		if (iy >= mem[(instruction & 0x00FFu)]) {
			sr |= 0b00000001u;
		}

		if (iy == mem[(instruction & 0x00FFu)]) {
			sr |= 0b00000010u;
		}

		break;

	case 0xD000: // BNE rel
		if (((sr >> 1) & 1) == 0) {
			pc += (instruction & 0x00FFu);
		}
		break;

	case 0xE000: // CPX #
		if (ix >= mem[(instruction & 0x00FFu)]) {
			sr |= 0b00000001u;
		}

		if (ix == mem[(instruction & 0x00FFu)]) {
			sr |= 0b00000010u;
		}
		
		break;

	case 0xF000: // BEQ rel
		if (((sr >> 1) & 1) == 1) {
			pc += (instruction & 0x00FFu);
		}
		break;

	default:
		break;
	}
}

RP2A03::~RP2A03()
{

}
