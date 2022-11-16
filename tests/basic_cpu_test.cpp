#include <climits>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <gtest/gtest.h>
#include <iostream>

// dirty solution, but needed to test private variables.
#define private public
#include "bus.hpp"
#include "cpu6502.hpp"
#undef private

auto cpu = std::make_unique<CPU6502>();
auto bus = std::make_shared<Bus>();
auto mem = std::make_shared<std::array<std::uint8_t, MEM_SIZE>>();

void prepare_mem() {
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

TEST(basic_cpu_test, load_store) {
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

	while (cpu->pc != 0x000F) {
		cpu->execute();
	}

	EXPECT_EQ(cpu->acc, (*mem)[0x0001]);
	EXPECT_EQ(cpu->x, (*mem)[0x0003]);
	EXPECT_EQ(cpu->y, (*mem)[0x0005]);
	EXPECT_EQ((*mem)[0xBBAA], cpu->acc);
	EXPECT_EQ((*mem)[0xBBAB], cpu->x);
	EXPECT_EQ((*mem)[0xBBAC], cpu->y);
}

TEST(basic_cpu_test, register_transfer) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// TAX IMP: transfer ACC to X register
	cpu->acc = std::rand() % std::numeric_limits<std::uint8_t>::max();
	(*mem)[0x0000] = 0xAA;
	while (cpu->pc != 0x0001) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->acc, cpu->x);

	// TAY IMP: transfer ACC to Y register
	cpu->acc = std::rand() % std::numeric_limits<std::uint8_t>::max();
	(*mem)[0x0001] = 0xA8;
	while (cpu->pc != 0x0002) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->acc, cpu->y);

	// TXA IMP: transfer X to ACC register
	cpu->x = std::rand() % std::numeric_limits<std::uint8_t>::max();
	(*mem)[0x0002] = 0x8A;
	while (cpu->pc != 0x0003) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->x, cpu->acc);

	// TYA IMP: transfer Y to ACC register
	cpu->y = std::rand() % std::numeric_limits<std::uint8_t>::max();
	(*mem)[0x0003] = 0x98;
	while (cpu->pc != 0x0004) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->y, cpu->acc);
}

TEST(basic_cpu_test, stack_ops) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, logical) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	std::uint8_t temp = std::rand() % std::numeric_limits<std::uint8_t>::max();
	cpu->acc = temp;

	// AND ABS: logical AND performed, bit-by-bit, using the memory location.
	(*mem)[0x0000] = 0x2D;
	(*mem)[0x0001] = 0x33;
	(*mem)[0x0002] = 0x44;
	(*mem)[0x4433] =
		std::rand() %
		std::numeric_limits<std::uint8_t>::max(); // byte to AND w/ ACC.

	while (cpu->pc != 0x0003) {
		cpu->execute();
	}

	EXPECT_EQ(cpu->acc,
			  temp & (*mem)[0x4433]); // check AND on ACC has been carried out.
	EXPECT_EQ(cpu->acc & 0b10000000,
			  (cpu->sr) & 0b10000000); // check N flag has been set correctly.

	temp = std::rand() % std::numeric_limits<std::uint8_t>::max();
	cpu->acc = temp;

	// EOR IMM: exclusive OR operation is performed bit-by-bit on the ACC and
	// contents of memory.
	(*mem)[0x0003] = 0x49;
	(*mem)[0x0004] = 0x55;

	while (cpu->pc != 0x0005) {
		cpu->execute();
	}

	EXPECT_EQ(cpu->acc,
			  temp ^ (*mem)[0x0004]); // check AND on ACC has been carried out.
	//	EXPECT_EQ(cpu->acc & 0b10000000, (cpu->sr) & 0b10000000); // check N
	//flag has been set correctly.
}

TEST(basic_cpu_test, arithmetic) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, inc_and_dec) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, shifts) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, jumps_and_calls) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	// JMP ABS: sets the program counter to the address supplied by the operand.
	(*mem)[0x0000] = 0x4C;
	(*mem)[0x0001] = 0x64;
	(*mem)[0x0002] = 0x99;

	while (cpu->pc < 0x0003) {
		cpu->execute();
	}

	EXPECT_EQ(cpu->pc, 0x9964);

	// JSR ABS: jumps to the subroutine of the given operand.
	(*mem)[0x9964] = 0x20;
	(*mem)[0x9965] = 0xAA;
	(*mem)[0x9966] = 0xCC;

	while (cpu->pc < 0x9977) {
		cpu->execute();
	}

	EXPECT_EQ(cpu->pc, 0xCCAA);
	// test stack
}

TEST(basic_cpu_test, branches) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);
}

TEST(basic_cpu_test, status_flag) {
	prepare_mem();

	// ensure that reset vector jump has worked properly.
	EXPECT_EQ(cpu->pc, 0x0000);

	(*mem)[0x0000] = 0x18; // CLC: clear carry flag.
	while (cpu->pc != 0x0001) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->sr & 0b00000001, 0);

	(*mem)[0x0001] = 0xD8; // CLD: clear decimal flag.
	while (cpu->pc != 0x0002) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->sr & 0b00001000, 0);

	(*mem)[0x0002] = 0x58; // CLI: clear interrupt disable flag.
	while (cpu->pc != 0x0003) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->sr & 0b00000100, 0);

	(*mem)[0x0003] = 0xB8; // CLV: clear overflow flag.
	while (cpu->pc != 0x0004) {
		cpu->execute();
	}
	EXPECT_EQ(cpu->sr & 0b01000000, 0);

	(*mem)[0x0004] = 0x38; // SEC: set carry flag.
	while (cpu->pc != 0x0005) {
		cpu->execute();
	}
	EXPECT_GT(cpu->sr & 0b00000001, 0);

	(*mem)[0x0005] = 0xF8; // SED: set decimal flag.
	while (cpu->pc != 0x0006) {
		cpu->execute();
	}
	EXPECT_GT(cpu->sr & 0b00001000, 0);

	(*mem)[0x0006] = 0x78; // SEI: set interrupt disable flag.
	while (cpu->pc != 0x0007) {
		cpu->execute();
	}
	EXPECT_GT(cpu->sr & 0b00000100, 0);
}

TEST(basic_cpu_test, system) {
	bus->set_mem(mem);
	bus->init_mem(); // clear memory from previous tests.

	// sets the reset vector to jump to address 0xDDAA.
	(*mem)[0xFFFC] = 0xAA;
	(*mem)[0xFFFD] = 0xDD;

	// connects the bus to the CPU and sets its registers to a known state.
	cpu->set_bus(bus);
	cpu->reset();

	// set to 0x0000 instead of the reset vector to test the BRK operation.
	cpu->pc = 0x0000;

	// BRK
	(*mem)[0x0000] = 0x00;

	// ?
	for (int i = 0; i != 8; ++i) {
		cpu->execute();
	}
}
