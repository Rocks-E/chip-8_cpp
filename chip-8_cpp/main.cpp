#include <iostream>
#include <cinttypes>
#include <SFML/Graphics.hpp>

#include "chip_8.hpp"

#define SCREEN_WIDTH DISPLAY_WIDTH * DEFAULT_SCALE_FACTOR
#define SCREEN_HEIGHT DISPLAY_HEIGHT * DEFAULT_SCALE_FACTOR
#define BIT_DEPTH 1

int32_t main(int32_t argc, const char * const *argv) {

	if(argc < 2) {
		std::cerr << "No file specified\n";
		exit(1);
	}



	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, BIT_DEPTH), "Chip-8");
	display monitor;
	chip_8 computer(argv[1], &monitor, 0xFFF);

	while(window.isOpen()) {
	
		sf::Event event;
		while(window.pollEvent(event))
			if(event.type == sf::Event::Closed)
				window.close();

		computer.run();

		window.clear();
		monitor.draw_display(window);
		window.display();

	}

	return 0;

}