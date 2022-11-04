#include "cartridge.hpp"

Cartridge::Cartridge(const std::filesystem::path &rom)
{
	this->loaded_cartridge = rom;
}

void Cartridge::start()
{
	// begin reading from the file.
}

Cartridge::~Cartridge()
{

}
