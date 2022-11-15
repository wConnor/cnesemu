#ifndef BUS_HPP
#define BUS_HPP

#include <array>
#include <memory>
#include <cstdint>

constexpr std::uint16_t MEM_SIZE = 0xFFFF;

class Bus
{
public:
	Bus();
	void init_mem();

	std::uint8_t read_byte(const std::uint16_t &addr);
	void write_byte(const std::uint16_t &addr, const std::uint8_t &data);

	std::uint16_t read_word(const std::uint16_t &addr);
	void write_word(const std::uint16_t &addr, const std::uint16_t &data);

	virtual ~Bus();

	void set_mem(std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> &mem);

private:
	std::shared_ptr<std::array<std::uint8_t, MEM_SIZE>> mem;
};

#endif
