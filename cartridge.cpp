#include "cartridge.hpp"

Cartridge::Cartridge(const std::filesystem::path &rom)
{
	this->loaded_cartridge = rom;
}

std::shared_ptr<std::array<std::uint8_t, MAX_CARTRIDGE_SIZE>> Cartridge::load()
{
	std::ifstream file(this->loaded_cartridge, std::ios::in | std::ios::binary);
	auto contents = std::make_shared<std::array<std::uint8_t, MAX_CARTRIDGE_SIZE>>();

	while (file.good()) {
		for (int i = 0; i != MAX_CARTRIDGE_SIZE; ++i) {
			file >> (*contents)[i];
		}
	}

	return contents;
}

Cartridge::~Cartridge()
{

}
