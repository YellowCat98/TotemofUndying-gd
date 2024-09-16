#pragma once

#include <Geode/Geode.hpp>

class TotemAnimation : public cocos2d::CCSprite {
private:
	std::function<void()> onFinished;
	bool m_shouldCleanup;
	bool init(std::function<void()> onceFinished, bool shouldCleanup);

public:
	static TotemAnimation* create(std::function<void()> onceFinished, bool shouldCleanup);
	void startAnimation(float dt);
	void FreeTheMemoryOfTheTotemAnimationClassWhichWouldComeInHandyInCertainCasesLikeWhenTheAnimationIsFinishedAndActuallyItsOnlyPurposeIsToGetFreedWhenTheAnimationIsDoneWhichIsPrettyCool();
};