#include <CCandyGenerator.h>

#include <Utils.h>
#include <vector>

TileType CCandyGenerator::GenerateCandy()
{
	std::vector<int> tileTypes;
	tileTypes.push_back(static_cast<int>(TileType::BLUE));
	tileTypes.push_back(static_cast<int>(TileType::GREEN));
	tileTypes.push_back(static_cast<int>(TileType::RED));
	tileTypes.push_back(static_cast<int>(TileType::YELLOW));
	tileTypes.push_back(static_cast<int>(TileType::PURPLE));
	return static_cast<TileType>(Utils::GetRandomIntFromVector(tileTypes));
}
