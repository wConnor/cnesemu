#include "nes.hpp"

NES::NES()
{

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
	// initialise CPU, Video, Sound, etc.

	spdlog::debug("NES powered on.");
}

void NES::power_off()
{
	cpu->clear();
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
