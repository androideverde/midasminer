#pragma once

class CScoringSystem
{
public:
	CScoringSystem();
	int AddToScore(int value);
	int GetScore() const { return mScore; }
private:
	int mScore = 0;
};
