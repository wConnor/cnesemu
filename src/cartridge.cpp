#include "cartridge.hpp"

// initialiser for if the user wishes to pass a string literal. primarily for
// debugging.
Cartridge::Cartridge(const std::string &temp)
{
	this->temp = temp;
}

// initialiser for passing a .BIN / .ROM file (such as a game or 6502 program).
Cartridge::Cartridge(const std::filesystem::path &rom)
{
	this->loaded_cartridge = rom;
}

// loads the contents of either the given ROM file or the string literal into
// the contents std::array for use by the NES.
std::vector<std::uint8_t> Cartridge::load()
{
	std::vector<std::uint8_t> contents;

	// implies that a ROM path has been passed instead of a string literal.
	if (this->temp.empty()) {
		std::ifstream file(this->loaded_cartridge, std::ios::binary);
		std::vector<std::uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
		contents = buffer;
	} else {
		for (int i = 0; i != temp.size(); ++i) {
			contents[i] = temp[i];
		}
	}

	return contents;
}

Cartridge::~Cartridge()
{
}
