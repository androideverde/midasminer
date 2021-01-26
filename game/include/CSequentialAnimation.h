#pragma once

#include <CAnimation.h>
#include <vector>
#include <memory>

class CSequentialAnimation
	: public CAnimation
{
public:
	CSequentialAnimation();
	void AddAnimation(std::unique_ptr<CAnimation> animation);
	void Update(float delta_time) override;
private:
	std::vector<std::unique_ptr<CAnimation>> mAnimations;
};
