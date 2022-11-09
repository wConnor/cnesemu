#include "cpu6502.hpp"

CPU6502::CPU6502()
{

}

void CPU6502::reset()
{
	acc = x = y = sp = sr = 0x00;
	addr_abs = 0x0040;
	pc = 0xFFFC;
	//	bus->init_mem();
	stack = std::stack<std::uint8_t>();
	cycles_left = 8;
	spdlog::debug("CPU reset.");
}

void CPU6502::execute()
{
	if (cycles_left == 0) {
		opcode = bus->read(pc);
		pc++;

		cycles_left = instr_matrix[opcode].cycles +
			addr_mode_map[instr_matrix[opcode].addr_mode]() +
			instr_map[instr_matrix[opcode].instr]();

		spdlog::debug("opcode=0x{:02x} ({} {}), pc=0x{:04x}, acc=0x{:02x}, x=0x{:02x}, y=0x{:02x}, sp=0x{:02x}, sr=0b{:08b}",
					  opcode, instr_matrix[opcode].instr, instr_matrix[opcode].addr_mode, pc, acc, x, y, sp, sr);
	}

	cycles_left--; // cycle elapsed
}

bool CPU6502::complete()
{
	return cycles_left == 0;
}

void CPU6502::set_bus(const std::shared_ptr<Bus> &bus)
{
	this->bus = bus;
}

/* **** ADDRESSING MODES **** */
std::uint8_t CPU6502::ABS()
{
	std::uint8_t lo = bus->read(pc++);
	std::uint8_t hi = bus->read(pc++);

	addr_abs = (hi << 8) | lo;

	return 0;
}

std::uint8_t CPU6502::ABX()
{
	std::uint8_t lo = bus->read(pc++);
	std::uint8_t hi = bus->read(pc++);
	addr_abs = ((hi << 8) | lo) + x;

	// checks for page boundary crossing; additional cycle if so.
	return (addr_abs & 0xFF00) != (hi << 8)
		? 1
		: 0;
}

std::uint8_t CPU6502::ABY()
{
	std::uint8_t lo = bus->read(pc++);
	std::uint8_t hi = bus->read(pc++);
	addr_abs = ((hi << 8) | lo) + y;

	// checks for page boundary crossing; additional cycle if so.
	return (addr_abs & 0xFF00) != (hi << 8)
		? 1
		: 0;
}

std::uint8_t CPU6502::ACC()
{
	fetched = acc;
	return 0;
}

std::uint8_t CPU6502::IND()
{
	std::uint16_t lo_ptr = bus->read(pc++);
	std::uint16_t hi_ptr = bus->read(pc++);

	std::uint16_t ptr = (hi_ptr << 8) | lo_ptr;
	addr_abs = (bus->read(ptr + 1) << 8) | bus->read(ptr);

	return 0;
}

std::uint8_t CPU6502::IZX()
{
	std::uint16_t t = bus->read(pc++);

	std::uint16_t lo = bus->read((t + x) & 0x00FF);
	std::uint16_t hi = bus->read((t + x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

std::uint8_t CPU6502::IZY()
{
	std::uint16_t t = bus->read(pc++);

	std::uint16_t lo = bus->read(t & 0x00FF);
	std::uint16_t hi = bus->read((t + 1) & 0x00FF);

	addr_abs = ((hi << 8) | lo) + y;

	// checks for page boundary crossing; additional cycle if so.
	return (addr_abs & 0xFF00) != (hi << 8)
		? 1
		: 0;
}

std::uint8_t CPU6502::IMP()
{
	fetched = acc;
	return 0;
}

std::uint8_t CPU6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

std::uint8_t CPU6502::ZPG()
{
	addr_abs = bus->read(pc++);
	addr_abs &= 0x00FF;
	return 0;
}

std::uint8_t CPU6502::ZPX()
{
	addr_abs = (bus->read(pc++) + x);
	addr_abs &= 0x00FF;
	return 0;
}

std::uint8_t CPU6502::ZPY()
{
	addr_abs = (bus->read(pc++) + y);
	addr_abs &= 0x00FF;
	return 0;
}

std::uint8_t CPU6502::REL()
{
	addr_rel = bus->read(pc++);

	if (addr_rel & 0x80) {
		addr_rel |= 0xFF00;
	}

	return 0;
}

/* **** INSTRUCTIONS **** */
std::uint8_t CPU6502::ADC()
{
	fetch();
	if (acc > 0 && fetched > std::numeric_limits<std::uint8_t>::max() - acc - (sr & (1 << 0))) {
		//sr |= 0b10000001; // definitely wrong; must study more.
	}

	acc += fetched + (sr & (1 << 0));

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 1;
}

std::uint8_t CPU6502::AND()
{
	fetch();
	acc &= fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	pc++;

	return 1;
}

std::uint8_t CPU6502::ASL()
{

	return 0;
}

std::uint8_t CPU6502::BCC()
{
	if ((sr & 0b00000001) == 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BCS()
{
	if ((sr & 0b00000001) != 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BEQ()
{
	if ((sr & 0b00000010) != 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BIT()
{
	fetch();

	return 0;
}

std::uint8_t CPU6502::BMI()
{
	if ((sr & 0b1000000) != 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BNE()
{
	if ((sr & 0b00000010) == 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BPL()
{
	if ((sr & 0b10000000) == 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BRK()
{
	stack.push(pc);
	stack.push(sr);
	pc = 0xFFFE;
	sr |= 0b00010000;

	return 0;
}

std::uint8_t CPU6502::BVC()
{
	if ((sr & 0b01000000) == 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BVS()
{
	if ((sr & 0b01000000) != 0) {
		cycles_left++;
		addr_abs = addr_rel + pc;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles_left++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::CLC()
{
	sr &= 0b11111110;
	return 0;
}

std::uint8_t CPU6502::CLD()
{
	sr &= 0b11110111;
	return 0;
}

std::uint8_t CPU6502::CLI()
{
	sr &= 0b11111011;
	return 0;
}

std::uint8_t CPU6502::CLV()
{
	sr &= 0b10111111;
	return 0;
}

std::uint8_t CPU6502::CMP()
{
	fetch();

	return 1;
}

std::uint8_t CPU6502::CPX()
{
	fetch();

	return 0;
}

std::uint8_t CPU6502::CPY()
{
	fetch();
	if (acc >= fetched) {
		sr |= 0b00000001;
	}

	if (acc == fetched) {
		sr |= 0b00000010;
	}

	if (((y - fetched)) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::DEC()
{
	fetch();
	bus->write(addr_abs, (fetched - 1) & 0x00FF);

	if (fetched - 1 == 0) {
		sr |= 0b00000010;
	}

	if (((fetched - 1) >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::DEX()
{
	x--;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::DEY()
{
	y--;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::EOR()
{
	fetch();
	acc ^= fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::INC()
{
	fetch();
	bus->write(addr_abs, (fetched + 1) & 0x00FF);

	if (fetched + 1 == 0) {
		sr |= 0b00000010;
	}

	if (((fetched + 1) >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::INX()
{
	x++;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::INY()
{
	y++;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::JMP()
{
	pc = addr_abs;
	return 0;
}

std::uint8_t CPU6502::JSR()
{
	stack.push(pc - 1);
	pc = addr_abs;

	return 0;
}

std::uint8_t CPU6502::LDA()
{
	fetch();
	acc = fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::LDX()
{
	fetch();
	x = fetched;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::LDY()
{
	fetch();
	y = fetched;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;

}

std::uint8_t CPU6502::LSR()
{

	return 0;
}

std::uint8_t CPU6502::NOP()
{
	return 0;
}

std::uint8_t CPU6502::ORA()
{
	fetch();
	acc |= fetched;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::PHA()
{
	stack.push(acc);
	return 0;
}

std::uint8_t CPU6502::PHP()
{
	stack.push(sr);
	return 0;
}

std::uint8_t CPU6502::PLA()
{
	acc = stack.top();
	stack.pop();

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::PLP()
{
	sr = stack.top();
	stack.pop();


	return 0;
}

std::uint8_t CPU6502::ROL()
{

	return 0;
}

std::uint8_t CPU6502::ROR()
{

	return 0;
}

std::uint8_t CPU6502::RTI()
{
	return 0;
}

std::uint8_t CPU6502::RTS()
{
	pc = stack.top();
	stack.pop();
	return 0;
}

std::uint8_t CPU6502::SBC()
{
	fetch();

	return 0;
}

std::uint8_t CPU6502::SEC()
{
	sr |= 0b00000001;
	return 0;
}

std::uint8_t CPU6502::SED()
{
	sr |= 0b00001000;
	return 0;
}

std::uint8_t CPU6502::SEI()
{
	sr |= 0b00000100;
	return 0;
}

std::uint8_t CPU6502::STA()
{
	bus->write(addr_abs, acc);
	return 0;
}

std::uint8_t CPU6502::STX()
{
	bus->write(addr_abs, x);
	return 0;
}

std::uint8_t CPU6502::STY()
{
	bus->write(addr_abs, y);
	return 0;
}

std::uint8_t CPU6502::TAX()
{
	x = acc;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::TAY()
{
	y = acc;

	if (y == 0) {
		sr |= 0b00000010;
	}

	if ((y >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::TSX()
{
	x = sp;

	if (x == 0) {
		sr |= 0b00000010;
	}

	if ((x >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::TXA()
{
	acc = x;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;
}

std::uint8_t CPU6502::TXS()
{
	sp = x;
	return 0;
}

std::uint8_t CPU6502::TYA()
{
	acc = y;

	if (acc == 0) {
		sr |= 0b00000010;
	}

	if ((acc >> 7) & 1) {
		sr |= 0b10000000;
	}

	return 0;

}

std::uint8_t CPU6502::ILL()
{
	return 0;
}

std::uint8_t CPU6502::fetch()
{
	if (instr_matrix[opcode].addr_mode != "IMP") {
		fetched = bus->read(addr_abs);
	}

	return fetched;
}

CPU6502::~CPU6502()
{

}
