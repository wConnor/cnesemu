#ifndef CPU6502_HPP
#define CPU6502_HPP

#include "fullinstruction.hpp"
#include "bus.hpp"

#include <spdlog/spdlog.h>

#include <map>
#include <cmath>
#include <bitset>
#include <string>
#include <stack>
#include <array>
#include <algorithm>
#include <memory>
#include <limits>

constexpr double CLOCK_SPEED_MHZ = 1.789773;
constexpr std::uint8_t STACK_SIZE = 255;
constexpr std::uint8_t INSTRUCTION_COUNT = 255;

class CPU6502
{
public:
	CPU6502();
	void fetch_fullinstruction(const std::uint8_t &op_code);
	void decode_instruction(const std::uint16_t &operand);
	void exec_instruction(const std::uint16_t &address);
	void clear();
	virtual ~CPU6502();

	void set_bus(const std::shared_ptr<Bus> &bus);
	std::uint8_t fetch();

	std::uint8_t read(const std::uint16_t &addr);
	void write(const std::uint16_t &addr, const std::uint8_t &data);

	void nes_init_regs();

	void clock_cycle();

private:
	/* registers: accumulator, x, y, stack pointer, status register, program counter
	   status register: bit 5 can not be changed and is always set to 1.
	   | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
	   | N | V | - | B | D | I | Z | C | */
	std::uint8_t acc = 0x00;
	std::uint8_t x = 0x00;
	std::uint8_t y = 0x00;
	std::uint8_t sp = 0x00;
	std::uint8_t sr = 0x00;
	std::uint16_t pc = 0x0000;

	std::stack<std::uint8_t> stack;

	FULLINSTRUCTION current_instruction;

	std::shared_ptr<Bus> bus = nullptr;
	std::uint8_t opcode = 0x00;
	std::uint8_t fetched = 0x00;
	std::uint16_t addr_abs = 0x0000;
	std::uint16_t addr_rel = 0x0000;
	std::uint8_t cycles = 0;

	std::unordered_map<std::string, std::function<void()>> addr_mode_map;
	std::unordered_map<std::string, std::function<void()>> instr_map;
	std::array<FULLINSTRUCTION, INSTRUCTION_COUNT> instr_matrix;

	// addressing modes
	void ABS();
	void ABX();
	void ABY();
	void ACC();
	void IND();
	void IZX();
	void IZY();
	void IMP();
	void IMM();
	void ZPG();
	void ZPX();
	void ZPY();
	void REL();

	// instructions
	void ADC();
	void AND();
	void ASL();
	void BCC();
	void BCS();
	void BEQ();
	void BIT();
	void BMI();
	void BNE();
	void BPL();
	void BRK();
	void BVC();
	void BVS();
	void CLC();
	void CLD();
	void CLI();
	void CLV();
	void CMP();
	void CPX();
	void CPY();
	void DEC();
	void DEX();
	void DEY();
	void EOR();
	void INC();
	void INX();
	void INY();
	void JMP();
	void JSR();
	void LDA();
	void LDX();
	void LDY();
	void LSR();
	void NOP();
	void ORA();
	void PHA();
	void PHP();
	void PLA();
	void PLP();
	void ROL();
	void ROR();
	void RTI();
	void RTS();
	void SBC();
	void SEC();
	void SED();
	void SEI();
	void STA();
	void STX();
	void STY();
	void TAX();
	void TAY();
	void TSX();
	void TXA();
	void TXS();
	void TYA();

	void ILL(); // illegal instruction
};

#endif
