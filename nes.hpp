#ifndef NES_HPP
#define NES_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "cpu6502.hpp"
#include "cartridge.hpp"
#include "video.hpp"

constexpr std::uint16_t MEMORY_SIZE = 8192;

class NES
{
public:
	NES();
	void insert_cartridge(const std::filesystem::path &rom);
	void power_on();
	void power_off();
	void reset();
	virtual ~NES();

private:
	std::unique_ptr<CPU6502> mos6502;
	std::unique_ptr<Cartridge> cartridge;
	std::unique_ptr<Video> video;

	std::shared_ptr<std::vector<std::uint8_t>> rom_contents;
};

#endif
