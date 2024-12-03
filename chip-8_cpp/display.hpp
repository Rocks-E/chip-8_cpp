#pragma once

#include <array>
#include <cinttypes>

#include <SFML/Graphics.hpp>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DEFAULT_SCALE_FACTOR 10

class display {
public:
	display(uint8_t scale_by = DEFAULT_SCALE_FACTOR);
	~display();

	void draw_display(sf::RenderWindow &draw_window) const;
	void clear_screen();

	void flip_pixel(uint8_t x, uint8_t y);

protected:

private:
	std::vector<sf::RectangleShape> pixels;
	sf::VertexArray grid_lines;

};
