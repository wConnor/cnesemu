#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <filesystem>
#include <fstream>
#include <vector>

class Cartridge
{
public:
	Cartridge(const std::filesystem::path &rom);
	std::shared_ptr<std::vector<std::uint8_t>> load();
	virtual ~Cartridge();

private:
	std::filesystem::path loaded_cartridge;
};

#endif
