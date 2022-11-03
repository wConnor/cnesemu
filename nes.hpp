#ifndef NES_HPP
#define NES_HPP

#include <cstdint>
#include <memory>

#include "mos6502.hpp"
#include "video.hpp"

constexpr std::uint16_t MEMORY_SIZE = 8192;

class NES
{
public:
	NES();
	virtual ~NES();

private:
	std::unique_ptr<MOS6502> mos6502;
	std::unique_ptr<Video> video;
};

#endif
