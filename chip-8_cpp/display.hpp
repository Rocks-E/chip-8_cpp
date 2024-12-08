#pragma once

#include <array>
#include <cinttypes>

#include <SFML/Graphics.hpp>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DEFAULT_SCALE_FACTOR 10
#define BIT_DEPTH 1

#define BLACK_PIXEL (0x00000000)
#define WHITE_PIXEL (0xFFFFFFFF)

class display {
public:
	display(uint8_t scale_by = DEFAULT_SCALE_FACTOR, bool enable_grid_lines = false);
	~display();

	void draw_display(sf::RenderWindow &window);
	void clear_screen();

	void flip_pixel(uint8_t x, uint8_t y);

	void toggle_grid_lines();

protected:

private:
	bool grid_lines_enable;
	sf::VertexArray grid_lines;

	sf::Sprite screen_sprite;
	sf::Texture screen_texture;
	uint8_t screen_pixels[DISPLAY_WIDTH * DISPLAY_HEIGHT * sizeof(int32_t)]{0};

	void update_display();

};
