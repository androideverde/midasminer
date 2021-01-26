#include <CSequentialAnimation.h>

CSequentialAnimation::CSequentialAnimation()
{
	mAnimations.clear();
}

void CSequentialAnimation::AddAnimation(std::unique_ptr<CAnimation> animation)
{
	mAnimations.push_back(std::move(animation));
}

void CSequentialAnimation::Update(float delta_time)
{
	if (mAnimations.empty())
	{
		mCompleted = true;
		return;
	}
	CAnimation& anim = *(mAnimations.at(0));
	anim.Update(delta_time);
	if (anim.IsCompleted())
	{
		mAnimations.erase(mAnimations.begin());
	}
}
