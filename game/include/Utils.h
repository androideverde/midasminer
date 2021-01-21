#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

namespace Utils
{
	SDL_Texture* LoadImage(const std::string& file_str, SDL_Renderer* renderer);
	SDL_Texture* LoadText(const std::string& text_str, TTF_Font* font, SDL_Renderer* renderer);
	int GetRandomIntFromVector(const std::vector<int> list);
}
