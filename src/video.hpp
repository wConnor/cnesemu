#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <SFML/Window.hpp>

#include <iostream>
#include <memory>
#include <array>

constexpr std::uint16_t SCREEN_WIDTH = 256;
constexpr std::uint16_t SCREEN_HEIGHT = 240;

constexpr std::uint16_t WINDOW_WIDTH = SCREEN_WIDTH * 3;
constexpr std::uint16_t WINDOW_HEIGHT = SCREEN_HEIGHT * 3;

class Video
{
public:
	Video();
	virtual ~Video();

	bool init();
	void update();

private:
	sf::Window window;
};

#endif
