#include <cstdint>
#include <gtest/gtest.h>

// dirty solution, but needed to test private variables.
#define private public
#include "cpu6502.hpp"
#include "bus.hpp"
#undef private

TEST(basic_cpu_test, simple_instructions)
{
	std::unique_ptr<CPU6502> cpu = std::make_unique<CPU6502>();
	std::shared_ptr<Bus> bus = std::make_shared<Bus>();
	std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> mem = std::make_shared<std::array<std::uint8_t, MEM_SIZE>>();
	// initialises the memory to 0x00.
	bus->set_mem(mem);

	// connects the bus to the CPU and sets its registers to a known state.
	cpu->set_bus(bus);
	cpu->reset();

	// sets the reset vector to jump to address 0x0000.
	(*mem)[0xFFFC] = 0x00;
	(*mem)[0xFFFD] = 0x00;

	// LDA IMM: loads the byte '0x49' into the ACC.
	(*mem)[0x0000] = 0xA9;
	(*mem)[0x0001] = 0x49;

	while (cpu->pc != 0x0002) {
		cpu->execute();
	}

	EXPECT_EQ(cpu->acc, 0x49);

	// (*mem)[0x0002] = 0xA2;
	// (*mem)[0x0003] = 0x03;

	// (*mem)[0x0004] = 0xE8;

	// (*mem)[0x0005] = 0x4C;
	// (*mem)[0x0006] = 0x04;
	// (*mem)[0x0007] = 0x55;
}
