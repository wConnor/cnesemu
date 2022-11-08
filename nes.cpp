#include "nes.hpp"

NES::NES()
{
	cpu = std::make_unique<CPU6502>();
	bus = std::make_shared<Bus>();
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
	std::fill(mem->begin(), mem->end(), 0x00);
	bus->set_mem(mem);
	cpu->set_bus(bus);
	cpu->nes_init_regs();

	// random test instructions
	cpu->fetch_fullinstruction(0x7D);
	cpu->decode_instruction(0xAA);

	cpu->decode_instruction(0x0A3D);
	cpu->decode_instruction(0x007E);
	cpu->decode_instruction(0xBC8C);
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
