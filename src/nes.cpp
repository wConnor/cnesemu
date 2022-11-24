#include "nes.hpp"

NES::NES()
{
	cpu = std::make_unique<CPU6502>();
	bus = std::make_shared<Bus>();
	mem = std::make_shared<std::array<std::uint8_t, MEM_SIZE>>();
	video = std::make_unique<Video>();
}

void NES::insert_cartridge(const std::filesystem::path &rom)
{
	cartridge = std::make_unique<Cartridge>(rom);
	spdlog::debug("Cartridge inserted from file {}.", std::string(rom));
}

void NES::eject_cartridge()
{
	cartridge = nullptr;
	spdlog::debug("Cartridge ejected. No ROM loaded.");
}

void NES::power_on()
{
	std::vector<std::uint8_t> buffer = cartridge->load();
	spdlog::debug("Loaded ROM of size {0:d} bytes.", buffer.size());
	std::move(buffer.begin(), buffer.end(), mem->begin());

	bus->set_mem(this->mem);
	cpu->set_bus(this->bus);
	cpu->reset();

	while (true) {
		cpu->execute();
	}

	// video->init();
}

void NES::power_off()
{
	// video->clear();
	//...
	spdlog::debug("NES powered off.");
}

void NES::reset()
{
	power_off();
	power_on();
}

NES::~NES()
{
}
