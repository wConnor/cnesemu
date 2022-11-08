#ifndef ADDRMODE_HPP
#define ADDRMODE_HPP

#include <cstdint>

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

	ILL, // illegal addressing mode
};

#endif
