#include <climits>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>

// dirty solution, but needed to test private variables.
#define private public
#include "bus.hpp"
#include "cpu6502.hpp"
#undef private

#define RAND_UINT8 std::rand() % std::numeric_limits<std::uint8_t>::max()

auto cpu = std::make_unique<CPU6502>();
auto bus = std::make_shared<Bus>();
auto mem = std::make_shared<std::array<std::uint8_t, MEM_SIZE>>();

void prepare_mem()
{
	std::srand(std::time(nullptr)); // random seed for testing

	bus->set_mem(mem); // connects the bus to the system memory.
	bus->init_mem();   // clear memory from previous tests.

	// sets the reset vector to jump to address 0x0000.
	(*mem)[0xFFFC] = 0x00;
	(*mem)[0xFFFD] = 0x00;

	// connects the bus to the CPU and sets its registers to a known state.
	cpu->set_bus(bus);
	cpu->reset();
}

void execute_until(const std::uint16_t &addr, const bool &jump)
{
	if (jump) {
		while (cpu->pc < addr) {
			cpu->execute();
		}
	} else {
		while (cpu->pc != addr) {
			cpu->execute();
		}
	}
}

TEST(basic_cpu_test, load_store)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// LDA IMM: loads the next byte into the ACC.
	(*mem)[0x0000] = 0xA9;
	(*mem)[0x0001] = 0x49;

	// LDX IMM: loads the next byte into the x register.
	(*mem)[0x0002] = 0xA2;
	(*mem)[0x0003] = 0x03;

	// LDY IMM: loads the next byte into the y register.
	(*mem)[0x0004] = 0xA0;
	(*mem)[0x0005] = 0x4C;

	// STA ABS: stores the contents of the ACC into the desired memory location.
	(*mem)[0x0006] = 0x8D;
	(*mem)[0x0007] = 0xAA;
	(*mem)[0x0008] = 0xBB;

	// STX ABS: stores the contents of the X register into the desired memory
	// location.
	(*mem)[0x0009] = 0x8E;
	(*mem)[0x000A] = 0xAB;
	(*mem)[0x000B] = 0xBB;

	// STY ABS: stores the contents of the Y register into the desired memory
	// location.
	(*mem)[0x000C] = 0x8C;
	(*mem)[0x000D] = 0xAC;
	(*mem)[0x000E] = 0xBB;

	execute_until(0x000F, false);
	EXPECT_EQ(cpu->acc, (*mem)[0x0001]);
	EXPECT_EQ(cpu->x, (*mem)[0x0003]);
	EXPECT_EQ(cpu->y, (*mem)[0x0005]);
	EXPECT_EQ((*mem)[0xBBAA], cpu->acc);
	EXPECT_EQ((*mem)[0xBBAB], cpu->x);
	EXPECT_EQ((*mem)[0xBBAC], cpu->y);
}

TEST(basic_cpu_test, register_transfer)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// TAX IMP: transfer ACC to X register
	cpu->acc = RAND_UINT8;
	(*mem)[0x0000] = 0xAA;
	execute_until(0x0001, false);
	EXPECT_EQ(cpu->acc, cpu->x);

	// TAY IMP: transfer ACC to Y register
	cpu->acc = RAND_UINT8;
	(*mem)[0x0001] = 0xA8;
	execute_until(0x0002, false);
	EXPECT_EQ(cpu->acc, cpu->y);

	// TXA IMP: transfer X to ACC register
	cpu->x = RAND_UINT8;
	(*mem)[0x0002] = 0x8A;
	execute_until(0x0003, false);
	EXPECT_EQ(cpu->x, cpu->acc);

	// TYA IMP: transfer Y to ACC register
	cpu->y = RAND_UINT8;
	(*mem)[0x0003] = 0x98;
	execute_until(0x0004, false);
	EXPECT_EQ(cpu->y, cpu->acc);
}

TEST(basic_cpu_test, stack_ops)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, logical)
{
	std::uint8_t temp;
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// AND ABS: logical AND performed, bit-by-bit, using the memory location.
	temp = RAND_UINT8;
	cpu->acc = temp; // random ACC value for testing.
	(*mem)[0x0000] = 0x2D;
	(*mem)[0x0001] = 0x33;
	(*mem)[0x0002] = 0x44;
	(*mem)[0x4433] = RAND_UINT8; // byte to AND w/ ACC.

	execute_until(0x0003, false);
	EXPECT_EQ(cpu->acc, temp & (*mem)[0x4433]); // check AND on ACC has been carried out.
	EXPECT_EQ(cpu->acc & 0b10000000, cpu->sr & 0b10000000); // check N flag.

	// EOR IMM: exclusive OR operation is performed bit-by-bit on the ACC and
	// contents of memory.
	temp = RAND_UINT8;
	cpu->acc = temp; // random ACC value for testing.
	(*mem)[0x0003] = 0x49;
	(*mem)[0x0004] = RAND_UINT8;

	execute_until(0x0005, false);
	EXPECT_EQ(cpu->acc, temp ^ (*mem)[0x0004]); // check EOR on ACC has been carried out.
	EXPECT_EQ(cpu->acc & 0b10000000, cpu->sr & 0b10000000); // check N flag.

	// ORA ABS: inclusive OR operation is performed bit-by-bit on the ACC and
	// contents of memory.
	temp = RAND_UINT8;
	cpu->acc = temp; // random ACC value for testing.
	(*mem)[0x0005] = 0x0D;
	(*mem)[0x0006] = 0x88;
	(*mem)[0x0007] = 0x55;
	(*mem)[0x5588] = RAND_UINT8;

	execute_until(0x0008, false);
	EXPECT_EQ(cpu->acc, temp | (*mem)[0x5588]); // check ORA on ACC has been carried out.
	EXPECT_EQ(cpu->acc & 0b10000000, cpu->sr & 0b10000000); // check N flag.

	// BIT ABS: tests if one or more bits are set in a target memory location.
	(*mem)[0x0008] = 0x2C;
	(*mem)[0x0009] = 0xFA;
	(*mem)[0x000A] = 0xAF;
	(*mem)[0xAFFA] = RAND_UINT8;

	execute_until(0x000B, false);

	if ((cpu->acc & (*mem)[0xAFFA]) == 0) {
		EXPECT_GT(cpu->sr & 0b00000010, 0);
	} else {
		EXPECT_EQ(cpu->sr & 0b00000010, 0);
	}

	EXPECT_EQ((*mem)[0xAFFA] & 0b11000000,
			  cpu->sr & 0b11000000); // N and V flag set to fetched result.
}

TEST(basic_cpu_test, arithmetic)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// ADC

	// SBC

	// CMP ABX: compares the contents of ACC with a value held in memory.
	cpu->acc = RAND_UINT8; // random ACC value.
	cpu->x = 0x05;	   // offset to test ABX addressing.
	(*mem)[0x0000] = 0xDD;
	(*mem)[0x0001] = 0x88;
	(*mem)[0x0002] = 0x99;
	(*mem)[0x9988 + cpu->x] = RAND_UINT8; // 0x9988 + 0x05 offset.

	execute_until(0x0003, false);
	if (cpu->acc - (*mem)[0x9988 + cpu->x] == 0) {
		EXPECT_GT(cpu->sr & 0b00000010, 0); // check Z flag set.
	} else {
		EXPECT_EQ(cpu->sr & 0b00000010, 0); // check Z flag clear.
	}

	if (cpu->acc >= (*mem)[0x9988 + cpu->x]) {
		EXPECT_GT(cpu->sr & 0b00000001, 0); // check C flag set.
	} else {
		EXPECT_EQ(cpu->sr & 0b00000001, 0); // check C flag clear.
	}

	EXPECT_EQ((cpu->acc - (*mem)[0x9988 + cpu->x]) & 0b10000000,
			  cpu->sr & 0b10000000); // check N flag.

	// CPX IMM: compares the contents of the x register with a value in memory.
	cpu->x = RAND_UINT8;
	(*mem)[0x0003] = 0xE0;
	(*mem)[0x0004] = RAND_UINT8;

	execute_until(0x0005, false);
	if (cpu->x - (*mem)[0x0004] == 0) {
		EXPECT_GT(cpu->sr & 0b00000010, 0); // check Z flag set.
	} else {
		EXPECT_EQ(cpu->sr & 0b00000010, 0); // check Z flag clear.
	}

	EXPECT_EQ(cpu->sr & 0b10000000, (cpu->x - (*mem)[0x0004]) & 0b10000000); // check N flag.
	if (cpu->x >= (*mem)[0x0004]) {
		EXPECT_GT(cpu->sr & 0b00000001, 0); // check C flag set.
	} else {
		EXPECT_EQ(cpu->sr & 0b00000001, 0); // check C flag clear.
	}

	EXPECT_EQ((cpu->x - (*mem)[0x0004]) & 0b10000000, cpu->sr & 0b10000000); // check N flag.

	// CPY ABS: compares the contents of the y register with a value in memory.
	cpu->y = RAND_UINT8;
	(*mem)[0x0005] = 0xCC;
	(*mem)[0x0006] = 0xBA;
	(*mem)[0x0007] = 0xBA;
	(*mem)[0xBABA] = RAND_UINT8;

	execute_until(0x0008, false);
	if (cpu->y - (*mem)[0xBABA] == 0) {
		EXPECT_GT(cpu->sr & 0b00000010, 0); // check Z flag set.
	} else {
		EXPECT_EQ(cpu->sr & 0b00000010, 0); // check Z flag clear.
	}

	if (cpu->y >= (*mem)[0xBABA]) {
		EXPECT_GT(cpu->sr & 0b00000001, 0); // check C flag.
	} else {
		EXPECT_EQ(cpu->sr & 0b00000001, 0); // check C flag.
	}

	EXPECT_EQ((cpu->y - (*mem)[0xBABA]) & 0b10000000, cpu->sr & 0b10000000); // check N flag.
}

TEST(basic_cpu_test, inc_and_dec)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// INC ABS: adds one to the value held at a memory location.
	std::uint8_t temp = RAND_UINT8;
	(*mem)[0x0000] = 0xEE;
	(*mem)[0x0001] = 0xAA;
	(*mem)[0x0002] = 0xBB;
	(*mem)[0xBBAA] = temp;

	execute_until(0x0003, false);
	EXPECT_EQ((*mem)[0xBBAA], temp + 1);

	// INX IMP: adds one to the X register.
	temp = cpu->x;
	(*mem)[0x0003] = 0xE8;

	execute_until(0x0004, false);
	EXPECT_EQ(cpu->x, temp + 1);

	// INY IMP: adds one to the Y register.
	temp = cpu->y;
	(*mem)[0x0004] = 0xC8;

	execute_until(0x0005, false);
	EXPECT_EQ(cpu->y, temp + 1);

	// DEC ABS: subtracts one from the value held at a memory location.
	temp = RAND_UINT8;
	(*mem)[0x0005] = 0xCE;
	(*mem)[0x0006] = 0xDE;
	(*mem)[0x0007] = 0xED;
	(*mem)[0xEDDE] = temp;

	execute_until(0x0008, false);
	EXPECT_EQ((*mem)[0xEDDE], temp - 1);

	// DEX IMP: subtracts one from the X register.
	temp = cpu->x;
	(*mem)[0x0008] = 0xCA;

	execute_until(0x0009, false);
	EXPECT_EQ(cpu->x, temp - 1);

	// DEY IMP: subtracts one from the Y register.
	temp = cpu->y;
	(*mem)[0x0009] = 0x88;

	execute_until(0x000A, false);
	EXPECT_EQ(cpu->y, temp - 1);
}

TEST(basic_cpu_test, shifts)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, jumps_and_calls)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// JMP ABS: sets the program counter to the address supplied by the operand.
	(*mem)[0x0000] = 0x4C;
	(*mem)[0x0001] = 0x64;
	(*mem)[0x0002] = 0x99;

	execute_until(0x0003, true);
	EXPECT_EQ(cpu->pc, 0x9964);

	// JSR ABS: jumps to the subroutine of the given operand.
	(*mem)[0x9964] = 0x20;
	(*mem)[0x9965] = 0xAA;
	(*mem)[0x9966] = 0xCC;

	execute_until(0x9967, true);
	EXPECT_EQ(cpu->pc, 0xCCAA);
	// test stack
}

TEST(basic_cpu_test, branches)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, status_flag)
{
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	(*mem)[0x0000] = 0x18; // CLC: clear carry flag.
	execute_until(0x0001, false);
	EXPECT_EQ(cpu->sr & 0b00000001, 0);

	(*mem)[0x0001] = 0xD8; // CLD: clear decimal flag.
	execute_until(0x0002, false);
	EXPECT_EQ(cpu->sr & 0b00001000, 0);

	(*mem)[0x0002] = 0x58; // CLI: clear interrupt disable flag.
	execute_until(0x0003, false);
	EXPECT_EQ(cpu->sr & 0b00000100, 0);

	(*mem)[0x0003] = 0xB8; // CLV: clear overflow flag.
	execute_until(0x0004, false);
	EXPECT_EQ(cpu->sr & 0b01000000, 0);

	(*mem)[0x0004] = 0x38; // SEC: set carry flag.
	execute_until(0x0005, false);
	EXPECT_GT(cpu->sr & 0b00000001, 0);

	(*mem)[0x0005] = 0xF8; // SED: set decimal flag.
	execute_until(0x0006, false);
	EXPECT_GT(cpu->sr & 0b00001000, 0);

	(*mem)[0x0006] = 0x78; // SEI: set interrupt disable flag.
	execute_until(0x0007, false);
	EXPECT_GT(cpu->sr & 0b00000100, 0);
}

TEST(basic_cpu_test, system)
{
	bus->set_mem(mem);
	bus->init_mem(); // clear memory from previous tests.

	// sets the reset vector to jump to address 0x0000.
	(*mem)[0xFFFC] = 0x99;
	(*mem)[0xFFFD] = 0x77;

	// connects the bus to the CPU and sets its registers to a known state.
	cpu->set_bus(bus);
	cpu->reset();

	// set to 0x0000 instead of the reset vector to test the BRK operation.
	cpu->pc = 0x7799;

	// BRK: forces the generation of an interrupt request.
	// https://www.c64-wiki.com/wiki/BRK
	(*mem)[0x7799] = 0x00;
	(*mem)[0x779A] = 0xEA; // NOP recommended after BRK due to pc + 2.

	// address for the BRK operation to jump to.
	(*mem)[0xFFFE] = 0xAA;
	(*mem)[0xFFFF] = 0xDD;

	execute_until(0x779A, true);
	EXPECT_EQ(cpu->pc, 0xDDAA);		// cpu->pc should have jumped to the new address.
	EXPECT_GT(cpu->sr & 0b00010000, 0); // break bit should be set to 1.

	// NOP: no operation; simply advance pc by 1.
	(*mem)[0xDDAA] = 0xEA;
	(*mem)[0xDDAB] = 0xB8; // random instruction to be executed next.

	execute_until(0xDDAC, false);
	EXPECT_EQ(cpu->opcode, (*mem)[0xDDAB]); // cpu->pc should have advanced.

	// RTI: return from an interrupt request.
	std::uint16_t old_pc = cpu->pc;
	(*mem)[0xDDAC] = 0x40;

	while (cpu->pc > 0xDDAB && cpu->pc < 0xDDAD) {
		cpu->execute();
	}

	EXPECT_NE(old_pc, cpu->pc);
	EXPECT_EQ(cpu->pc, 0x779B); // BRK increments the pc by 2 instead of 1.
}
