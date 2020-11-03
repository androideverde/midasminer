#include <CAnimationSystem.h>

CAnimationSystem::CAnimationSystem()
{
	mAnimationQueue.clear();
}

void CAnimationSystem::AddAnimation(CAnimation animation)
{
	mAnimationQueue.push_back(animation);
}

void CAnimationSystem::CurrentAnimationComplete()
{
	mAnimationQueue.erase(mAnimationQueue.begin());
}
