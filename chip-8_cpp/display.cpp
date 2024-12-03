#include "display.hpp"

display::display(uint8_t scale_by) {

	this->pixels.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT);

	this->grid_lines.setPrimitiveType(sf::Lines);
	for(size_t c = 1; c < DISPLAY_WIDTH; c++) {
		this->grid_lines.append(sf::Vertex(sf::Vector2f(c * scale_by, 0), sf::Color(0x88, 0x88, 0x88)));
		this->grid_lines.append(sf::Vertex(sf::Vector2f(c * scale_by, DISPLAY_HEIGHT * scale_by), sf::Color(0x88, 0x88, 0x88)));
	}
	for(size_t c = 1; c < DISPLAY_HEIGHT; c++) {
		this->grid_lines.append(sf::Vertex(sf::Vector2f(0, c * scale_by), sf::Color(0x88, 0x88, 0x88)));
		this->grid_lines.append(sf::Vertex(sf::Vector2f(DISPLAY_WIDTH * scale_by, c * scale_by), sf::Color(0x88, 0x88, 0x88)));
	}

	// Move all pixels to their designated positions, start as black
	for(size_t row = 0; row < DISPLAY_HEIGHT; row++) {

		size_t rect_pos = DISPLAY_WIDTH * row;

		for(size_t col = 0; col < DISPLAY_WIDTH; col++) {

			this->pixels[rect_pos].setSize(sf::Vector2f(scale_by, scale_by));
			this->pixels[rect_pos].setPosition(col * scale_by, row * scale_by);
			this->pixels[rect_pos].setFillColor(sf::Color::Black);

			rect_pos++;

		}
	}

}
display::~display() {}

void display::draw_display(sf::RenderWindow &draw_window) const {
	
	for(sf::RectangleShape rect : this->pixels)
		draw_window.draw(rect);

	draw_window.draw(this->grid_lines);

}
void display::clear_screen() {
	for(sf::RectangleShape &rect : this->pixels)
		rect.setFillColor(sf::Color::Black);
}

void display::flip_pixel(uint8_t x, uint8_t y) {
	
	if(x > DISPLAY_WIDTH || y > DISPLAY_HEIGHT)
		return;

	size_t pixel_pos = DISPLAY_WIDTH * y + x;

	// Swap the color of this pixel
	this->pixels[pixel_pos].setFillColor((this->pixels[pixel_pos].getFillColor() == sf::Color::Black) ? sf::Color::White : sf::Color::Black);

}