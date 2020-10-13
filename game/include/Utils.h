#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

namespace Utils
{
	SDL_Texture* LoadImage(const std::string& file_str, SDL_Renderer* renderer);
	int GetRandomIntFromVector(const std::vector<int> list);
}
