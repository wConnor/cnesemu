#ifndef RP2A03_HPP
#define RP2A03_HPP

#include <array>
#include <tuple>
#include <cstdint>

class RP2A03
{
public:
	RP2A03();
	virtual ~RP2A03();
private:
	/* registers: accumlator, x index, y index, flag, stack pointer, program counter (16-bit) */
	std::tuple<std::uint8_t, std::uint8_t, std::uint8_t,
			   std::uint8_t, std::uint8_t, std::uint16_t> regs;

};

#endif
