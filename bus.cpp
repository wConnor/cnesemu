#include "bus.hpp"

Bus::Bus()
{

}

void Bus::init_mem()
{
	std::fill(mem->begin(), mem->end(), 0x00);
}

std::uint8_t Bus::read(const std::uint16_t &addr)
{
	return (*mem)[addr];
}

void Bus::write(const std::uint16_t &addr, const std::uint8_t &data)
{
	(*mem)[addr] = data;
}

Bus::~Bus()
{

}

void Bus::set_mem(std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> &mem)
{
	this->mem = mem;
}
