#include <CScoringSystem.h>

CScoringSystem::CScoringSystem()
{
}

int CScoringSystem::AddToScore(int value)
{
	mScore += value;
	return mScore;
}
