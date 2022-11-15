#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <filesystem>
#include <fstream>
#include <array>

constexpr std::uint16_t MAX_CARTRIDGE_SIZE = 0xFFFF;

class Cartridge
{
public:
	Cartridge(const std::string &contents);
	Cartridge(const std::filesystem::path &rom);
	std::shared_ptr<std::array<std::uint8_t, MAX_CARTRIDGE_SIZE>> load();
	virtual ~Cartridge();

private:
	std::filesystem::path loaded_cartridge;
	std::shared_ptr<std::array<std::uint8_t, MAX_CARTRIDGE_SIZE>> contents;
	std::string temp = "";
};

#endif
