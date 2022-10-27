#ifndef RP2A03_HPP
#define RP2A03_HPP

#include <bitset>
#include <string>
#include <stack>
#include <array>
#include <cstdint>

constexpr std::uint16_t MEM_SIZE = 2048;
constexpr std::uint8_t STACK_SIZE = 255;

class RP2A03
{
public:
	RP2A03();
	void exec_instruction(std::uint16_t &instruction);
	virtual ~RP2A03();

private:
	// 0xX0 instructions
	void OP_BRK(const std::uint8_t &operand);
	void OP_BPL(const std::uint8_t &operand);
	void OP_JSR(const std::uint8_t &operand);
	void OP_BMI(const std::uint8_t &operand);
	void OP_RTI(const std::uint8_t &operand);
	void OP_BVC(const std::uint8_t &operand);
	void OP_RTS(const std::uint8_t &operand);
	void OP_BVS(const std::uint8_t &operand);
	void OP_BCC(const std::uint8_t &operand);
	void OP_LDY(const std::uint8_t &operand);
	void OP_BCS(const std::uint8_t &operand);
	void OP_CPY(const std::uint8_t &operand);
	void OP_BNE(const std::uint8_t &operand);
	void OP_CPX(const std::uint8_t &operand);
	void OP_BEQ(const std::uint8_t &operand);

	// 0xX1 instructions
	void OP_ORA(const std::uint8_t &operand);
	void OP_AND(const std::uint8_t &operand);
	void OP_EOR(const std::uint8_t &operand);
	void OP_ADC(const std::uint8_t &operand);
	void OP_STA(const std::uint8_t &operand);
	void OP_LDA(const std::uint8_t &operand);
	void OP_CMP(const std::uint8_t &operand);
	void OP_SBC(const std::uint8_t &operand);

	// 0xX2 instructions
	void OP_LDX(const std::uint8_t &operand);

	// 0xX4 instructions
	void OP_BIT(const std::uint8_t &operand);
	void OP_STY(const std::uint8_t &operand);

	// 
	

	/* registers: accumulator, x index, y index, stack pointer, status register, program counter
	   status register: bit 5 can not be changed and is always set to 1. */
	std::uint8_t acc, ix, iy, sp, sr;
	std::uint16_t pc;

	std::stack<std::uint8_t> stack;
	std::array<std::uint8_t, MEM_SIZE> mem;

};

#endif
