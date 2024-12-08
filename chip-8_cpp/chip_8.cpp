#include "chip_8.hpp"

const std::array<uint8_t, 0x50> FONT_ARRAY = {
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

const std::array<sf::Keyboard::Scancode, 0x10> KEYBOARD_CODES = {
	sf::Keyboard::Scancode::X,
	sf::Keyboard::Scancode::Num1,
	sf::Keyboard::Scancode::Num2,
	sf::Keyboard::Scancode::Num3,
	sf::Keyboard::Scancode::Q,
	sf::Keyboard::Scancode::W,
	sf::Keyboard::Scancode::E,
	sf::Keyboard::Scancode::A,
	sf::Keyboard::Scancode::S,
	sf::Keyboard::Scancode::D,
	sf::Keyboard::Scancode::Z,
	sf::Keyboard::Scancode::C,
	sf::Keyboard::Scancode::Num4,
	sf::Keyboard::Scancode::R,
	sf::Keyboard::Scancode::F,
	sf::Keyboard::Scancode::V
};

chip_8::chip_8(std::string rom_filename, display *display_ptr, uint16_t init_stack_pointer) : display_ptr(display_ptr), program_counter(0x200), index_register(0), stack_pointer(init_stack_pointer) {

	std::copy(FONT_ARRAY.begin(), FONT_ARRAY.end(), this->memory.begin() + FONT_START);

	std::ifstream rom_file(rom_filename, std::ios::binary);

	if(!rom_file.is_open()) 
		throw std::invalid_argument("Could not open ROM file");

	size_t file_size;

	rom_file.seekg(0, std::ios::end);
	file_size = rom_file.tellg();
	rom_file.seekg(0, std::ios::beg);

	rom_file.read(reinterpret_cast<char *>(this->memory.data() + 0x200), file_size);

	if(!rom_file) {
	
		rom_file.close();

		throw std::ios::failure("Could not read from file");

	}

	rom_file.close();

	std::srand(time(NULL));
	
}
chip_8::~chip_8() {

}

void chip_8::run() {

	sf::Time elapsed_time(sf::microseconds(0));
	sf::Time frame_time(sf::seconds(1 / 60.f));
	sf::Time cycle_time(sf::microseconds(1));

	sf::Clock chip_clock;

	while (elapsed_time < frame_time) {

		elapsed_time += chip_clock.getElapsedTime();
		chip_clock.restart();

		this->decode_and_execute(this->fetch());

		sf::sleep(cycle_time);

	}

	this->update_timers();

}

uint8_t chip_8::parse_instruction(const uint16_t &instruction, uint8_t &x, uint8_t &y, uint8_t &n, uint8_t &nn, uint16_t &nnn) {

	x = (instruction >> 8) & 0x0F;
	y = (instruction >> 4) & 0x0F;
	n = instruction & 0x0F;
	nn = instruction & 0xFF;
	nnn = instruction & 0x0FFF;

	// Opcode
	return instruction >> 12;

}

void chip_8::push(uint16_t addr) {
	this->memory[this->stack_pointer--] = addr & 0xFF;
	this->memory[this->stack_pointer--] = addr >> 8;
}
uint16_t chip_8::pop() {
	
	uint16_t stack_val = this->memory[++this->stack_pointer] << 8;
	stack_val |= this->memory[++this->stack_pointer];

	return stack_val;

}

void chip_8::update_timers() {
	
	if(this->sound_timer) {
		this->sound_timer--;
		this->play_sound();
	}

	if(this->delay_timer)
		this->delay_timer--;

}
void chip_8::play_sound() {}

uint16_t chip_8::fetch() {

	uint16_t instruction_data = (this->memory[this->program_counter] << 8) | this->memory[this->program_counter + 1];
	this->program_counter += 2;

	return instruction_data;

}
void chip_8::decode_and_execute(uint16_t instruction) {

	uint8_t x, y, n, nn;
	uint16_t nnn;

	switch(chip_8::parse_instruction(instruction, x, y, n, nn, nnn)) {
	
		case 0x0:
		
			// $00E0 -> Clear screen
			if(nnn == 0x0E0)
				this->display_ptr->clear_screen();
			else if(nnn == 0x0EE)
				this->program_counter = this->pop();

			break;
		
		// JUMP nnn
		case 0x1:
		
			this->program_counter = nnn;
		
			break;
		
		case 0x2:

			this->push(this->program_counter);
			this->program_counter = nnn;
		
			break;
		
		case 0x3:
		
			if(this->registers[x] == nn)
				this->program_counter += 2;

			break;
		
		case 0x4:

			if(this->registers[x] != nn)
				this->program_counter += 2;
		
			break;
		
		case 0x5:

			if(this->registers[x] == this->registers[y])
				this->program_counter += 2;
		
			break;
		
		case 0x6:
		
			this->registers[x] = nn;
		
			break;
		
		case 0x7:
		
			this->registers[x] += nn;
		
			break;
		
		case 0x8:
		
			switch(n) {
				
				case 0x0:

					this->registers[x] = this->registers[y];

					break;

				case 0x1:

					this->registers[x] |= this->registers[y];

					break;

				case 0x2:

					this->registers[x] &= this->registers[y];

					break;

				case 0x3:

					this->registers[x] ^= this->registers[y];

					break;

				case 0x4:

					{

						uint16_t temp = this->registers[x] + this->registers[y];

						this->registers[0xF] = (temp > 0xFF) ? 1 : 0;
						this->registers[x] = temp;

					}

					break;

				case 0x5:

					{
					
						uint16_t temp = this->registers[x] - this->registers[y];

						this->registers[0xF] = (temp > 0xFF) ? 0 : 1;
						this->registers[x] = temp & 0xFF;

					}

					break;

				case 0x6:

#if defined(_COSMAC)				
					this->registers[x] = this->registers[y];
#endif

					this->registers[0xF] = this->registers[x] & 1;
					this->registers[x] >>= 1;

					break;

				case 0x7:

					{
					
						uint16_t temp = this->registers[y] - this->registers[x];

						this->registers[0xF] = (temp > 0xFF) ? 0 : 1;
						this->registers[x] = temp & 0xFF;

					}

					break;

				case 0xE:

#if defined(_COSMAC)				
					this->registers[x] = this->registers[y];
#endif

					this->registers[0xF] = this->registers[x] >> 7;
					this->registers[x] <<= 1;

					break;

			}

			break;
		
		case 0x9:
		
			if(this->registers[x] != this->registers[y])
				this->program_counter += 2;

			break;
		
		case 0xA:
		
			this->index_register = nnn;
		
			break;
		
		case 0xB:

			{

				uint16_t jump_addr = nnn;

#if defined(_COSMAC)				
				jump_addr += this->registers[0];
#else
				jump_addr += this->registers[x];
#endif

				this->program_counter = jump_addr;

			}
		
			break;
		
		case 0xC:

			this->registers[x] = std::rand() & nn;
		
			break;
		
		case 0xD:
		
			{
			
				uint16_t vx = this->registers[x] % DISPLAY_WIDTH;
				uint16_t vy = this->registers[y] % DISPLAY_HEIGHT;

				this->registers[0xF] = 0;

				for(size_t c = 0; c < n; c++) {
					uint8_t mem_byte = this->memory[this->index_register + c];
					for(uint8_t i = 0; i < 8; i++) {
						if((mem_byte >> (7 - i)) & 1)
							this->display_ptr->flip_pixel(vx + i, vy + c);
					}
				}
			
			}

			break;
		
		case 0xE:
		
			if(nn == 0x9E) {
			
				if(sf::Keyboard::isKeyPressed(KEYBOARD_CODES[this->registers[x]]))
					this->program_counter += 2;

			}
			else if(nn == 0xA1) {
			
				if(!sf::Keyboard::isKeyPressed(KEYBOARD_CODES[this->registers[x]]))
					this->program_counter += 2;

			}

			break;
		
		case 0xF:

			switch(nn) {
			
				case 0x07:

					this->registers[x] = this->delay_timer;

					break;

				case 0x0A:

					{
						uint8_t key_num = 0xFF;

						for(uint8_t c = 0; c < KEYBOARD_CODES.size(); c++)
							if(sf::Keyboard::isKeyPressed(KEYBOARD_CODES[c])) {
								key_num = c;
								break;
							}

						if(key_num == 0xFF)
							this->program_counter -= 2;
						else
							this->registers[x] = key_num;

					}

					break;

				case 0x15:

					this->delay_timer = this->registers[x];

					break;

				case 0x18:

					this->sound_timer = this->registers[x];

					break;

				case 0x1E:

					{
						uint16_t new_index = this->index_register + this->registers[x];
						this->index_register = new_index;

#if !defined(_COSMAC)
						this->registers[0xF] = new_index > 0xFFF;
#endif
					}

					break;

				case 0x29:

					{
						uint8_t offset = 5;

						this->index_register = 5 * (this->registers[x]
#if defined(_COSMAC)
						& 0xF
#endif
						) + FONT_START;

					}

					break;

				case 0x33:

					{

						uint8_t vx = this->registers[x];

						this->memory[this->index_register + 2] = vx % 10;
						vx /= 10;
						this->memory[this->index_register + 1] = vx % 10;
						vx /= 10;
						this->memory[this->index_register] = vx;

					}

					break;

				case 0x55:

					for(uint8_t c = 0; c <= x; c++)
#if defined(_COSMAC)
						this->memory[this->index_register++] = this->registers[c];
#else
						this->memory[this->index_register + c] = this->registers[c];
#endif

				case 0x65:

					for(uint8_t c = 0; c <= x; c++)
#if defined(_COSMAC)
						this->registers[c] = this->memory[this->index_register++];
#else
						this->registers[c] = this->memory[this->index_register + c];
#endif

			}
		
			break;

	}

}