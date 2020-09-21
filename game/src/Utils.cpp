#include <Utils.h>

SDL_Texture* Utils::LoadImage(const std::string& file_str, SDL_Renderer* renderer)
{
	const char* file = file_str.c_str();
	SDL_Surface* surf = IMG_Load(file);
	if (surf == nullptr)
	{
		printf( "Unable to load image %s! SDL Error: %s\n", file, SDL_GetError() );
		return nullptr;
	}
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	surf = nullptr;
	return tex;
}
