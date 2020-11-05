#include <CAnimationSystem.h>

CAnimationSystem::CAnimationSystem()
{
	mAnimationQueue.clear();
}

void CAnimationSystem::AddAnimation(std::unique_ptr<CAnimation> animation)
{
	mAnimationQueue.push_back(std::move(animation));
}

void CAnimationSystem::CurrentAnimationComplete()
{
	mAnimationQueue.erase(mAnimationQueue.begin());
}
