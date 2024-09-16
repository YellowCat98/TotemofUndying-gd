#pragma once

#include <Geode/Geode.hpp>

class TotemAnimation : public cocos2d::CCSprite {
private:
    std::function<void()> onFinished;
    bool init(std::function<void()> onceFinished);
public:
    static TotemAnimation* create(std::function<void()> onceFinished);
    void startAnimation(float dt);
    void FreeTheMemoryOfTheTotemAnimationClassWhichWouldComeInHandyInCertainCasesLikeWhenTheAnimationIsFinishedAndActuallyItsOnlyPurposeIsToGetFreedWhenTheAnimationIsDoneWhichIsPrettyCool();
};