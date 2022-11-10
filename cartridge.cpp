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
std::shared_ptr<std::array<std::uint8_t, MAX_CARTRIDGE_SIZE>> Cartridge::load()
{
	// implies that a ROM path has been passed instead of a string literal.
	if (this->temp.empty()) {
		std::ifstream file(this->loaded_cartridge, std::ios::in | std::ios::binary);
		this->contents = std::make_shared<std::array<std::uint8_t, MAX_CARTRIDGE_SIZE>>();

		while (file.good()) {
			for (int i = 0; i != MAX_CARTRIDGE_SIZE; ++i) {
				file >> (*this->contents)[i];
			}
		}
	} else {
		for (int i = 0; i != temp.size(); ++i) {
			(*this->contents)[i] = temp[i];
		}
	}

	return this->contents;
}

Cartridge::~Cartridge()
{

}
