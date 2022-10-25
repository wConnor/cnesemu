#include "video.hpp"

Video::Video()
{
	std::fill(memory.begin(), memory.end(), 0x00);
}

bool Video::init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
	}

	this->win = SDL_CreateWindow("CHIP-8", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (this->win == nullptr) {
		std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
	}

	this->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (this->renderer == nullptr) {
		std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
	}

}

Video::~Video()
{

}
