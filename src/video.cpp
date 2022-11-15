#include "video.hpp"

Video::Video()
{

}

bool Video::init()
{
	window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "cnesemu");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		sf::Event e;

		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.display();
	}

	return true;
}

void Video::update()
{

}

Video::~Video()
{

}
