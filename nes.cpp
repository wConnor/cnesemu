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
	rom_contents = nullptr;
	spdlog::debug("Cartridge ejected. No ROM loaded.");
}

void NES::power_on()
{
	rom_contents = cartridge->load();
	spdlog::debug("Loaded ROM of size {0:d} bytes.", rom_contents->size());

	bus->set_mem(this->rom_contents);
	cpu->set_bus(this->bus);
	cpu->reset();

	do {
		cpu->execute();
	} while (true);
	//	} while (!cpu->complete());

	// video->init();
}

void NES::power_off()
{
	//video->clear();
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
