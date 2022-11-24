#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <vector>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <array>

class Cartridge
{
public:
	Cartridge(const std::string &contents);
	Cartridge(const std::filesystem::path &rom);
	std::vector<std::uint8_t> load();
	virtual ~Cartridge();

private:
	std::filesystem::path loaded_cartridge;
	std::string temp = "";
};

#endif
