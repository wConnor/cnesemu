#ifndef CARTRIDGE_HPP
#define CARTRIDGE_HPP

#include <filesystem>

class Cartridge
{
public:
	Cartridge(const std::filesystem::path &rom);
	void start();
	virtual ~Cartridge();

private:
	std::filesystem::path loaded_cartridge;
};

#endif
