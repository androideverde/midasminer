#include <CCandyGenerator.h>

#include <Utils.h>
#include <vector>

CandyType CCandyGenerator::GenerateCandy() const
{
	std::vector<int> tileTypes;
	tileTypes.push_back(static_cast<int>(CandyType::BLUE));
	tileTypes.push_back(static_cast<int>(CandyType::GREEN));
	tileTypes.push_back(static_cast<int>(CandyType::RED));
	tileTypes.push_back(static_cast<int>(CandyType::YELLOW));
	tileTypes.push_back(static_cast<int>(CandyType::PURPLE));
	return static_cast<CandyType>(Utils::GetRandomIntFromVector(tileTypes));
}
