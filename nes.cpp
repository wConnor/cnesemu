#include "nes.hpp"
#include <ios>

NES::NES()
{

}

void NES::insert_cartridge(const std::filesystem::path &rom)
{
	this->cartridge = std::make_unique<Cartridge>(rom);
}

void NES::power_on()
{
	rom_contents = this->cartridge->load();

	// initialise CPU, Video, Sound, etc.
}

void NES::power_off()
{

}

void NES::reset()
{

}

NES::~NES()
{

}
