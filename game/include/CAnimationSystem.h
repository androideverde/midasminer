#pragma once

#include <CAnimation.h>
#include <vector>

class CAnimationSystem
{
public:
	CAnimationSystem();
	void AddAnimation(std::unique_ptr<CAnimation> animation);
	bool IsEmpty() const { return mAnimationQueue.empty(); }
	CAnimation& GetNextAnimation() { return *(mAnimationQueue.at(0)); }
	void CurrentAnimationComplete();
private:
	std::vector<std::unique_ptr<CAnimation>> mAnimationQueue;
};
