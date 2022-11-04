#include "cartridge.hpp"

Cartridge::Cartridge(const std::filesystem::path &rom)
{
	this->loaded_cartridge = rom;
}

std::shared_ptr<std::vector<std::uint8_t>> Cartridge::load()
{
	std::ifstream file(this->loaded_cartridge, std::ios::in | std::ios::binary);
	auto contents = std::make_shared<std::vector<std::uint8_t>>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	return contents;
}

Cartridge::~Cartridge()
{

}
