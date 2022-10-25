#ifndef VIDEO_HPP
#define VIDEO_HPP

#include <iostream>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>

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
	std::array <uint8_t, SCREEN_WIDTH * SCREEN_HEIGHT> memory;
	SDL_Window *win;
	SDL_Renderer *renderer;
};

#endif
