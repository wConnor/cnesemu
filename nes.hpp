#ifndef NES_HPP
#define NES_HPP

#include <cstdint>
#include <memory>
#include "rp2a03.hpp"

constexpr std::uint16_t MEMORY_SIZE = 8192;

class NES
{
public:
	NES();
	virtual ~NES();

private:
	std::unique_ptr<RP2A03> rp2a03;
};

#endif
