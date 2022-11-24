#include <memory>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

#define private public
#include "cartridge.hpp"
#include "bus.hpp"
#include "cpu6502.hpp"
#undef private

TEST(functional_test, run_functional_test_bin)
{
	auto cpu = std::make_unique<CPU6502>();
	auto bus = std::make_shared<Bus>();
	auto cartridge = std::make_unique<Cartridge>(std::filesystem::path("./6502_functional_test.bin"));
	auto mem = std::make_shared<std::array<std::uint8_t, MEM_SIZE>>();

	spdlog::set_level(spdlog::level::debug);

	std::vector<std::uint8_t> buffer = cartridge->load();
	std::move(buffer.begin(), buffer.end(), mem->begin());

	bus->set_mem(mem);
	cpu->set_bus(bus);
	cpu->pc = 0x0400; // starting address for the functional test.

	EXPECT_EQ((*mem)[cpu->pc], 0xD8); // first instruction to be executed.

	while (true) {
		cpu->execute();
	}
}
