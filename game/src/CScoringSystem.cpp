#include <CScoringSystem.h>

#include <cstdio>

CScoringSystem::CScoringSystem()
{
}

int CScoringSystem::AddToScore(int value)
{
	printf("Previous score: %d\n", mScore);
	mScore += value * mMultiplier;
	printf("Adding Score: %d x combo level %d => %d\n", value, mMultiplier, mScore);
	return mScore;
}

int CScoringSystem::IncreaseMultiplier(int value)
{
	mMultiplier += value;
	return mMultiplier;
}

void CScoringSystem::ResetMultiplier()
{
	mMultiplier = 1;
}
