#include "bus.hpp"

Bus::Bus()
{

}

void Bus::init_mem()
{
	std::fill(mem->begin(), mem->end(), 0x00);
}

std::uint8_t Bus::read_byte(const std::uint16_t &addr)
{
	return (*mem)[addr];
}

void Bus::write_byte(const std::uint16_t &addr, const std::uint8_t &data)
{
	(*mem)[addr] = data;
}

std::uint16_t Bus::read_word(const std::uint16_t &addr)
{
	std::uint8_t lo = this->read_byte(addr);
	std::uint8_t hi = this->read_byte(addr + 1);

	return lo | (hi << 8);
}

void Bus::write_word(const std::uint16_t &addr, const std::uint16_t &data)
{
	(*mem)[addr] = data & 0xFF;
	(*mem)[addr + 1] = (data >> 8);
}

Bus::~Bus()
{

}

void Bus::set_mem(std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> &mem)
{
	this->mem = mem;
}
