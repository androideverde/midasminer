#pragma once

class CScoringSystem
{
public:
	CScoringSystem();
	int AddToScore(int value);
	int GetScore() const { return mScore; }
	int IncreaseMultiplier(int value);
	void ResetMultiplier();
	int GetMultiplier() const { return mMultiplier; }
private:
	int mScore = 0;
	int mMultiplier = 1;
};
