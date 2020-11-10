#include <CParallelAnimation.h>

CParallelAnimation::CParallelAnimation(float duration, std::vector<std::unique_ptr<CAnimation>> parallelAnimations)
	: CAnimation(duration)
	, mAnimations(std::move(parallelAnimations))
{
}

void CParallelAnimation::Update(float delta_time)
{
	int count = 0;
	for (auto& anim : mAnimations)
	{
		if (!anim->IsCompleted())
		{
			anim->Update(delta_time);
		}
		else
		{
			count++;
		}
	}
	if (mAnimations.size() == count)
	{
		mCompleted = true;
	}
}
