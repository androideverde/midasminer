#include <Utils.h>

#include <random>

SDL_Texture* Utils::LoadImage(const std::string& file_str, SDL_Renderer* renderer)
{
	if (file_str == "")
	{
		printf("Empty file name for image to load\n");
		return nullptr;
	}
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

int Utils::GetRandomIntFromVector(const std::vector<int> list)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_int_distribution<int> distribution(0, static_cast<int>(list.size()) - 1);
	int randomPick = distribution(generator);
	return list[randomPick];
}
