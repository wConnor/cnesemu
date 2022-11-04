#include "nes.hpp"
#include <string>
#include <iostream>

int main(int argc, char *argv[])
{
	std::unique_ptr<NES> nes = std::make_unique<NES>();

	if (argc == 1) {
		std::cerr << "Path to ROM not specified.";
		return 1;
	}

	nes->insert_cartridge(std::filesystem::path(static_cast<std::string>(argv[1])));
	nes->power_on();

	return 0;
}
