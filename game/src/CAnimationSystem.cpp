#include <CAnimationSystem.h>

CAnimationSystem::CAnimationSystem()
{
	mAnimationQueue.clear();
}

void CAnimationSystem::AddAnimation(std::unique_ptr<CAnimation> animation)
{
	mAnimationQueue.push_back(std::move(animation));
}

void CAnimationSystem::Update(float delta_time)
{
	if (mAnimationQueue.empty())
	{
		return;
	}
	CAnimation& anim = *(mAnimationQueue.at(0));
	anim.Update(delta_time);
	if (anim.IsCompleted())
	{
		mAnimationQueue.erase(mAnimationQueue.begin());
	}
}
