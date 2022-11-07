#include "nes.hpp"

NES::NES()
{
	cpu = std::make_unique<CPU6502>();
	mem = std::make_shared<std::array<std::uint8_t, MEM_SIZE>>();
}

void NES::insert_cartridge(const std::filesystem::path &rom)
{
	cartridge = std::make_unique<Cartridge>(rom);
	spdlog::debug("Cartridge inserted from file {}.", std::string(rom));
}

void NES::eject_cartridge()
{
	cartridge = nullptr;
	rom_contents = nullptr;
	spdlog::debug("Cartridge ejected. No ROM loaded.");
}

void NES::power_on()
{
	rom_contents = cartridge->load();
	spdlog::debug("Loaded ROM of size {0:d} bytes.", rom_contents->size());
	cpu->set_mem(mem);
	cpu->nes_init_regs();
	//video->set_mem(mem);

	// enter FDE cycle loop.
	cpu->decode_instruction(0x7Du);
	cpu->decode_instruction(0x3Du);
	cpu->decode_instruction(0x7Eu);
	cpu->decode_instruction(0x8Cu);
}

void NES::power_off()
{
	cpu->clear();
	//video->clear();
	//...
	spdlog::debug("NES powered off.");
}

void NES::reset()
{
	power_off();
	power_on();
}

NES::~NES()
{

}
