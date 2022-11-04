#include "nes.hpp"

NES::NES()
{

}

void NES::insert_cartridge(const std::filesystem::path &rom)
{
	this->cartridge = std::make_unique<Cartridge>(rom);
}

void NES::power_on()
{

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
