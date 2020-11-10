#pragma once

#include <CAnimation.h>
#include <vector>
#include <memory>

class CAnimationSystem
{
public:
	CAnimationSystem();
	void AddAnimation(std::unique_ptr<CAnimation> animation);
	bool AllAnimationsCompleted() const { return mAnimationQueue.empty(); }
	void Update(float delta_time);
private:
	std::vector<std::unique_ptr<CAnimation>> mAnimationQueue;
};
