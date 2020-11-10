#pragma once

#include <CAnimation.h>
#include <vector>
#include <memory>

class CParallelAnimation
	: public CAnimation
{
public:
	CParallelAnimation(float duration, std::vector<std::unique_ptr<CAnimation>> parallelAnimations);
	void Update(float delta_time) override;
private:
	std::vector<std::unique_ptr<CAnimation>> mAnimations;
};
