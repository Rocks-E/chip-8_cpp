#include <iostream>
#include <cinttypes>
#include <SFML/Graphics.hpp>

#include "chip_8.hpp"



int32_t main(int32_t argc, const char * const *argv) {

#ifndef _DEBUG
	if(argc < 2) {
		std::cerr << "No file specified\n";
		exit(1);
	}
#endif

	sf::RenderWindow window(sf::VideoMode(DISPLAY_WIDTH * DEFAULT_SCALE_FACTOR, DISPLAY_HEIGHT * DEFAULT_SCALE_FACTOR, BIT_DEPTH), "Chip-8");
	display monitor(DEFAULT_SCALE_FACTOR, true);
#ifndef _DEBUG
	chip_8 computer(argv[1], &monitor, 0xFFF);
#else
	chip_8 computer("C:/Users/minec/Downloads/chip8-master/Sierpinski [Sergey Naydenov, 2010].ch8", &monitor, 0xFFF);
#endif

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