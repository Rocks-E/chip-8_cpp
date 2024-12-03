#pragma once

#include <string>
#include <array>
#include <fstream>
#include <exception>
#include <algorithm>
#include <cinttypes>
#include <cstdlib>

#include "display.hpp"

#ifndef FONT_START
#define FONT_START 0x50
#endif

class chip_8 {

public:
	chip_8(std::string rom_filename, display *display_ptr, uint16_t init_stack_pointer);
	~chip_8();

	void run();

protected:

private:
	uint8_t delay_timer{0};
	uint8_t sound_timer{0};
	uint16_t program_counter : 12;
	// Empty stack, grows down
	uint16_t stack_pointer : 12;
	uint16_t index_register : 12;
	std::array<uint8_t, 0x1000> memory{0};
	std::array<uint8_t, 0x10> registers{0};
	display *display_ptr;

	static uint8_t parse_instruction(const uint16_t &instruction, uint8_t &x, uint8_t &y, uint8_t &n, uint8_t &nn, uint16_t &nnn);

	void push(uint16_t addr);
	uint16_t pop();

	void update_timers();
	void play_sound();

	uint16_t fetch();
	void decode_and_execute(uint16_t instruction);

};
