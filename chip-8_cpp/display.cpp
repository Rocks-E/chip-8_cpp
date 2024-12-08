#include "display.hpp"

display::display(uint8_t scale_by, bool enable_grid_lines) : grid_lines_enable(enable_grid_lines) {

	this->screen_texture.create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
	this->screen_sprite.setScale(scale_by, scale_by);

	this->grid_lines.setPrimitiveType(sf::Lines);
	for(size_t c = 1; c < DISPLAY_WIDTH; c++) {
		this->grid_lines.append(sf::Vertex(sf::Vector2f(c * scale_by, 0), sf::Color(0x88, 0x88, 0x88)));
		this->grid_lines.append(sf::Vertex(sf::Vector2f(c * scale_by, DISPLAY_HEIGHT * scale_by), sf::Color(0x88, 0x88, 0x88)));
	}
	for(size_t c = 1; c < DISPLAY_HEIGHT; c++) {
		this->grid_lines.append(sf::Vertex(sf::Vector2f(0, c * scale_by), sf::Color(0x88, 0x88, 0x88)));
		this->grid_lines.append(sf::Vertex(sf::Vector2f(DISPLAY_WIDTH * scale_by, c * scale_by), sf::Color(0x88, 0x88, 0x88)));
	}

}
display::~display() {}

void display::update_display() {
	
	this->screen_texture.update(this->screen_pixels);
	this->screen_sprite.setTexture(this->screen_texture);

}

void display::draw_display(sf::RenderWindow &window) {
	
	this->update_display();

	window.draw(this->screen_sprite);
	if(this->grid_lines_enable)
		window.draw(this->grid_lines);

}
void display::clear_screen() {
	for (size_t c = 0; c < (DISPLAY_WIDTH * DISPLAY_HEIGHT); c++)
		(uint32_t &)(this->screen_pixels[c * sizeof(int32_t)]) = BLACK_PIXEL;
}

void display::flip_pixel(uint8_t x, uint8_t y) {
	
	if(x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT)
		return;

	(uint32_t &)(this->screen_pixels[(DISPLAY_WIDTH * y + x) * sizeof(int32_t)]) ^= 0xFFFFFFFF;

}

void display::toggle_grid_lines() {
	this->grid_lines_enable = !this->grid_lines_enable;
}