#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>

enum class INSTRUCTION : std::uint8_t {
	ADC, // add with carry
	AND, // AND with ACC
	ASL, // arithmetic shift left
	BCC, // branch on carry clear
	BCS, // branch on carry set
	BEQ, // branch on equal (zero set)
	BIT, // bit test
	BMI, // branch on minus (negative set)
	BNE, // branch on not equal (zero clear)
	BPL, // branch on plus (negative clear)
	BRK, // break / interrupt
	BVC, // branch on overflow clear
	BVS, // branch on overflow set
	CLC, // clear carry
	CLD, // clear decimal
	CLI, // clear interrupt disable
	CLV, // clear overflow
	CMP, // compare with ACC
	CPX, // compare with index X
	CPY, // compare with index Y
	DEC, // decrement
	DEX, // decrement index X
	DEY, // decrement index Y
	EOR, // XOR with ACC
	INC, // increment
	INX, // increment index X
	INY, // increment index Y
	JMP, // jump
	JSR, // jump subroutine
	LDA, // load ACC
	LDX, // load index X
	LDY, // load index Y
	LSR, // logical shift right
	NOP, // no operation
	ORA, // OR with ACC
	PHA, // push ACC
	PHP, // push status register (SR)
	PLA, // pull ACC
	PLP, // pull status register (SR)
	ROL, // rotate left
	ROR, // rotate right
	RTI, // return from interrupt
	RTS, // return from subroutine
	SBC, // subtract with carry
	SEC, // set carry
	SED, // set decimal
	SEI, // set interrupt disable
	STA, // store ACC
	STX, // store index X
	STY, // store index Y
	TAX, // transfer ACC to index X
	TAY, // transfer ACC to index Y
	TSX, // transfer SP to index X
	TXA, // transfer index X to ACC
	TXS, // transfer index X to SP
	TYA, // transfer index Y to ACC

	ILL, // illegal opcode
};

#endif
