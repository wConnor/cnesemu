#include "cpu6502.hpp"

CPU6502::CPU6502()
{
}

void CPU6502::reset()
{
	acc = x = y = 0x00;
	sr = 0b00100000;
	addr_abs = 0x0000;
	pc = 0xFFFC;
	std::uint8_t lo = fetch_byte(), hi = fetch_byte();
	pc = (hi << 8) | lo;
	sp = 0xFF;

	spdlog::debug("CPU reset. pc=0x{:04x}", pc);
	cycles = 8;
}

void CPU6502::execute()
{
	if (cycles == 0) {
		opcode = this->fetch_byte();

		spdlog::debug("opcode=0x{:02x} ({} {}), pc=0x{:04x}, acc=0x{:02x}, "
					  "x=0x{:02x}, y=0x{:02x}, sp=0x{:02x}, sr=0b{:08b}",
					  opcode, instr_matrix[opcode].instr, instr_matrix[opcode].addr_mode,
					  pc - 0x0001, acc, x, y, sp, sr);

		cycles = instr_matrix[opcode].cycles + addr_mode_map[instr_matrix[opcode].addr_mode]() +
			instr_map[instr_matrix[opcode].instr]();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(55));
	cycles--;
}

std::uint8_t CPU6502::fetch()
{
	if (instr_matrix[opcode].addr_mode != "IMP") {
		fetched = bus->read_byte(addr_abs);
	}

	return fetched;
}

std::uint8_t CPU6502::read_byte(const std::uint16_t &addr)
{
	std::uint8_t data = bus->read_byte(addr);
	cycles--;
	return data;
}

void CPU6502::write_byte(const std::uint16_t &addr, const std::uint8_t &data)
{
	bus->write_byte(addr, data);
}

std::uint16_t CPU6502::read_word(const std::uint16_t &addr)
{
	std::uint16_t data = bus->read_word(addr);
	cycles -= 2;
	return data;
}

void CPU6502::write_word(const std::uint16_t &addr, const std::uint16_t &data)
{
	bus->write_word(addr, data);
}

std::uint8_t CPU6502::fetch_byte()
{
	std::uint8_t data = this->read_byte(pc);
	pc++;
	return data;
}

std::uint16_t CPU6502::fetch_word()
{
	std::uint16_t data = this->read_word(pc);
	pc += 2;
	return data;
}

void CPU6502::set_clear_negative_flag(const std::uint8_t &value)
{
	(value & 0b10000000) > 0 ? sr |= 0b10000000 : sr &= 0b01111111;
}

void CPU6502::set_clear_zero_flag(const std::uint8_t &value)
{
	value == 0 ? sr |= 0b00000010 : sr &= 0b11111101;
}

void CPU6502::stack_push(const std::uint8_t &value)
{
	write_byte(0x0100 + sp, value);
	sp--;
}

std::uint8_t CPU6502::stack_pop()
{
	sp++;
	return read_byte(0x0100 + sp);
}

bool CPU6502::complete()
{
	return cycles == 0;
}

void CPU6502::set_bus(const std::shared_ptr<Bus> &bus)
{
	this->bus = bus;
}

/* **** ADDRESSING MODES **** */
std::uint8_t CPU6502::ABS()
{
	std::uint8_t lo = this->fetch_byte();
	std::uint8_t hi = this->fetch_byte();
	addr_abs = (hi << 8) | lo;

	return 0;
}

std::uint8_t CPU6502::ABX()
{
	std::uint8_t lo = this->fetch_byte();
	std::uint8_t hi = this->fetch_byte();

	addr_abs = ((hi << 8) | lo) + x;

	// checks for page boundary crossing; additional cycle if so.
	return (addr_abs & 0xFF00) != (hi << 8) ? 1 : 0;
}

std::uint8_t CPU6502::ABY()
{
	std::uint8_t lo = this->fetch_byte();
	std::uint8_t hi = this->fetch_byte();

	addr_abs = ((hi << 8) | lo) + y;

	// checks for page boundary crossing; additional cycle if so.
	return (addr_abs & 0xFF00) != (hi << 8) ? 1 : 0;
}

std::uint8_t CPU6502::ACC()
{
	fetched = acc;
	return 0;
}

std::uint8_t CPU6502::IND()
{
	std::uint8_t lo_ptr = this->fetch_byte();
	std::uint8_t hi_ptr = this->fetch_byte();

	std::uint16_t ptr = (hi_ptr << 8) | lo_ptr;

	addr_abs = lo_ptr == 0x00FF ? (this->read_byte(ptr & 0xFF00) << 8) | this->read_byte(ptr)
		: (this->read_byte(ptr + 1) << 8) | this->read_byte(ptr);

	return 0;
}

std::uint8_t CPU6502::IZX()
{
	std::uint8_t tmp = this->fetch_byte();

	std::uint8_t lo = this->read_byte((tmp + x) & 0x00FF);
	std::uint8_t hi = this->read_byte((tmp + x + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

std::uint8_t CPU6502::IZY()
{
	std::uint8_t tmp = this->fetch_byte();

	std::uint8_t lo = this->read_byte((tmp + y) & 0x00FF);
	std::uint8_t hi = this->read_byte((tmp + y + 1) & 0x00FF);

	addr_abs = (hi << 8) | lo;

	return 0;
}

std::uint8_t CPU6502::IMP()
{
	return 0;
}

std::uint8_t CPU6502::IMM()
{
	addr_abs = pc++;
	return 0;
}

std::uint8_t CPU6502::ZPG()
{
	addr_abs = this->fetch_word();
	addr_abs &= 0x00FF;
	return 0;
}

std::uint8_t CPU6502::ZPX()
{
	addr_abs = (this->fetch_word() + x);
	addr_abs &= 0x00FF;
	return 0;
}

std::uint8_t CPU6502::ZPY()
{
	addr_abs = (this->fetch_word() + y);
	addr_abs &= 0x00FF;
	return 0;
}

std::uint8_t CPU6502::REL()
{
	addr_rel = this->fetch_byte();

	if (addr_rel & 0b10000000) {
		addr_rel |= 0xFF00;
	}

	return 0;
}

/* **** INSTRUCTIONS **** */
std::uint8_t CPU6502::ADC()
{
	fetch();
	if (acc > 0 && fetched > std::numeric_limits<std::uint8_t>::max() - acc - (sr & (1 << 0))) {
		// sr |= 0b10000001; // definitely wrong; must study more.
	}

	acc += fetched + (sr & (1 << 0));

	set_clear_zero_flag(acc);

	if ((acc & 0b10000000) != 0) {
		sr |= 0b10000000;
	}

	return 1;
}

std::uint8_t CPU6502::AND()
{
	fetch();
	acc &= fetched;

	set_clear_zero_flag(acc);

	if ((acc & 0b10000000) != 0) {
		sr |= 0b10000000;
	}

	return 1;
}

std::uint8_t CPU6502::ASL()
{
	fetch();
	fetched <<= 1;

	if ((fetched & 0xFF00) > 0) {
		sr |= 0b00000001;
	}

	set_clear_zero_flag(fetched);
	set_clear_negative_flag(fetched);

	if (instr_matrix[opcode].addr_mode == "IMP") {
		acc = fetched;
	} else {
		write_byte(addr_abs, fetched);
	}

	return 0;
}

std::uint8_t CPU6502::BCC()
{
	if ((sr & 0b00000001) == 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BCS()
{
	if ((sr & 0b00000001) > 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BEQ()
{
	if ((sr & 0b00000010) > 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BIT()
{
	fetch();

	set_clear_zero_flag(acc & fetched);

	// ugly solution, but works.
	(fetched >> 7) & 1 ? sr |= 0b10000000 : sr &= 0b01111111;
	(fetched >> 6) & 1 ? sr |= 0b01000000 : sr &= 0b10111111;

	return 0;
}

std::uint8_t CPU6502::BMI()
{
	if ((sr & 0b10000000) > 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BNE()
{
	if ((sr & 0b00000010) == 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BPL()
{
	if ((sr & 0b10000000) == 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BRK()
{
	pc++;

	stack_push((pc >> 8) & 0xFF);
	stack_push(pc & 0xFF);
	stack_push(sr);

	pc = 0xFFFE;
	std::uint8_t lo = fetch_byte(), hi = fetch_byte();
	pc = (hi << 8) | lo;
	sr |= 0b00010000;

	return 0;
}

std::uint8_t CPU6502::BVC()
{
	if ((sr & 0b01000000) == 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
		}

		pc = addr_abs;
	}

	return 0;
}

std::uint8_t CPU6502::BVS()
{
	if ((sr & 0b01000000) > 0) {
		cycles++;

		addr_abs = pc + addr_rel;

		// checks for page boundary crossing, adding an additional cycle.
		if ((addr_abs & 0xFF00) != (pc & 0xFF00)) {
			cycles++;
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
	std::uint8_t result = acc - fetched;

	acc >= fetched ? sr |= 0b00000001 : sr &= 0b11111110;
	set_clear_zero_flag(result);
	set_clear_negative_flag(result);

	return 1;
}

std::uint8_t CPU6502::CPX()
{
	fetch();
	std::uint8_t result = x - fetched;

	x >= fetched ? sr |= 0b00000001 : sr &= 0b11111110;
	set_clear_zero_flag(result);
	set_clear_negative_flag(result);

	return 0;
}

std::uint8_t CPU6502::CPY()
{
	fetch();
	std::uint8_t result = y - fetched;

	y >= fetched ? sr |= 0b00000001 : sr &= 0b11111110;
	set_clear_zero_flag(result);
	set_clear_negative_flag(result);

	return 0;
}

std::uint8_t CPU6502::DEC()
{
	fetch();
	this->write_word(addr_abs, (fetched - 1) & 0x00FF);

	set_clear_zero_flag(fetched - 1);
	set_clear_negative_flag(fetched - 1);

	return 0;
}

std::uint8_t CPU6502::DEX()
{
	x--;

	set_clear_zero_flag(x);
	set_clear_negative_flag(x);

	return 0;
}

std::uint8_t CPU6502::DEY()
{
	y--;

	set_clear_zero_flag(y);
	set_clear_negative_flag(y);

	return 0;
}

std::uint8_t CPU6502::EOR()
{
	fetch();
	acc ^= fetched;

	set_clear_zero_flag(acc);
	set_clear_negative_flag(acc);

	return 0;
}

std::uint8_t CPU6502::INC()
{
	fetch();
	this->write_word(addr_abs, fetched + 1);

	set_clear_zero_flag(fetched + 1);
	set_clear_negative_flag(fetched + 1);

	return 0;
}

std::uint8_t CPU6502::INX()
{
	x++;

	set_clear_zero_flag(x);
	set_clear_negative_flag(x);

	return 0;
}

std::uint8_t CPU6502::INY()
{
	y++;

	set_clear_zero_flag(y);
	set_clear_negative_flag(y);

	return 0;
}

std::uint8_t CPU6502::JMP()
{
	pc = addr_abs;
	return 0;
}

std::uint8_t CPU6502::JSR()
{
	pc--;

	stack_push((pc >> 8) & 0xFF);
	stack_push(pc & 0xFF);

	pc = addr_abs;

	return 0;
}

std::uint8_t CPU6502::LDA()
{
	fetch();
	acc = fetched;

	set_clear_zero_flag(acc);
	set_clear_negative_flag(acc);

	return 0;
}

std::uint8_t CPU6502::LDX()
{
	fetch();
	x = fetched;

	set_clear_zero_flag(x);
	set_clear_negative_flag(x);

	return 0;
}

std::uint8_t CPU6502::LDY()
{
	fetch();
	y = fetched;

	set_clear_zero_flag(y);
	set_clear_negative_flag(y);

	return 0;
}

std::uint8_t CPU6502::LSR()
{
	fetch();
	fetched >>= 1;

	// TODO: double-check reference.
	if ((fetched & 0xFF00) > 0) {
		sr |= 0b00000001;
	}

	set_clear_zero_flag(fetched);
	set_clear_negative_flag(fetched);

	if (instr_matrix[opcode].addr_mode == "IMP") {
		acc = fetched;
	} else {
		write_byte(addr_abs, fetched);
	}

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

	set_clear_zero_flag(acc);
	set_clear_negative_flag(acc);

	return 0;
}

std::uint8_t CPU6502::PHA()
{
	stack_push(acc);
	return 0;
}

std::uint8_t CPU6502::PHP()
{
	stack_push(sr);
	return 0;
}

std::uint8_t CPU6502::PLA()
{
	acc = stack_pop();

	set_clear_zero_flag(acc);
	set_clear_negative_flag(acc);

	return 0;
}

std::uint8_t CPU6502::PLP()
{
	sr = stack_pop();

	return 0;
}

std::uint8_t CPU6502::ROL()
{
	fetch();
	fetched <<= 1;

	if ((fetched & 0xFF00) > 0) {
		sr |= 0b00000001;
	}

	set_clear_zero_flag(fetched);
	set_clear_negative_flag(fetched);

	if (instr_matrix[opcode].addr_mode == "IMP") {
		acc = fetched;
	} else {
		write_byte(addr_abs, fetched);
	}

	return 0;
}

std::uint8_t CPU6502::ROR()
{
	fetch();
	std::uint8_t temp = fetched >> 1;

	if ((fetched & 0xFF00) > 0) {
		sr |= 0b00000001;
	}

	set_clear_zero_flag(fetched);
	set_clear_negative_flag(fetched);

	if (instr_matrix[opcode].addr_mode == "IMP") {
		acc = fetched;
	} else {
		write_byte(addr_abs, fetched);
	}

	return 0;
}

std::uint8_t CPU6502::RTI()
{
	sr = stack_pop();

	std::uint8_t lo = stack_pop();
	std::uint8_t hi = stack_pop();
	pc = (hi << 8) | lo;

	return 0;
}

std::uint8_t CPU6502::RTS()
{
	std::uint8_t lo = stack_pop();
	std::uint8_t hi = stack_pop();
	pc = (hi << 8) | lo;
	pc++;

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
	this->write_byte(addr_abs, acc);
	return 0;
}

std::uint8_t CPU6502::STX()
{
	this->write_byte(addr_abs, x);
	return 0;
}

std::uint8_t CPU6502::STY()
{
	this->write_byte(addr_abs, y);
	return 0;
}

std::uint8_t CPU6502::TAX()
{
	x = acc;

	set_clear_zero_flag(x);
	set_clear_negative_flag(x);

	return 0;
}

std::uint8_t CPU6502::TAY()
{
	y = acc;

	set_clear_zero_flag(y);
	set_clear_negative_flag(y);

	return 0;
}

std::uint8_t CPU6502::TSX()
{
	x = sp;

	set_clear_zero_flag(x);
	set_clear_negative_flag(x);

	return 0;
}

std::uint8_t CPU6502::TXA()
{
	acc = x;

	set_clear_zero_flag(acc);
	set_clear_negative_flag(acc);

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

	set_clear_zero_flag(acc);
	set_clear_negative_flag(acc);

	return 0;
}

std::uint8_t CPU6502::ILL()
{
	return 0;
}

/* **** OTHER OPERATIONS **** */
void CPU6502::IRQ()
{
	// if I flag (bit 2) is clear, interrupts are allowed.
	if ((sr & 0b00000000) == 0) {
	}
}

void CPU6502::NMI()
{
}

CPU6502::~CPU6502()
{
}
