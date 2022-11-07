#ifndef NES_HPP
#define NES_HPP

#include "cpu6502.hpp"
#include "cartridge.hpp"
#include "video.hpp"

class NES
{
public:
	NES();
	void insert_cartridge(const std::filesystem::path &rom);
	void eject_cartridge();
	void power_on();
	void power_off();
	void reset();
	virtual ~NES();

private:
	std::unique_ptr<CPU6502> cpu;
	std::unique_ptr<Cartridge> cartridge;
	std::unique_ptr<Video> video;

	std::shared_ptr<std::vector<std::uint8_t>> rom_contents;
	std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> mem;
};

#endif
