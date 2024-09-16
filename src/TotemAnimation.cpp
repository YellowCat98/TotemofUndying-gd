#include "TotemAnimation.hpp"

using namespace geode::prelude;

bool TotemAnimation::init(std::function<void()> onceFinished, bool shouldCleanup) {
    if (!CCSprite::init()) return false;
    this->setScale(2.850f); // set scale cause 1 looks small

    onFinished = onceFinished;
    m_shouldCleanup = shouldCleanup;

    this->schedule(schedule_selector(TotemAnimation::startAnimation), (0.3f/12.0f));
    
    return true;
}

void TotemAnimation::FreeTheMemoryOfTheTotemAnimationClassWhichWouldComeInHandyInCertainCasesLikeWhenTheAnimationIsFinishedAndActuallyItsOnlyPurposeIsToGetFreedWhenTheAnimationIsDoneWhichIsPrettyCool() {
    if (this->getParent()) {
        this->removeFromParent();
    }
    this->release();
}

void TotemAnimation::startAnimation(float dt) {
    static int frame = 1;
    if (frame == 67) {
        frame = 1;
        onFinished();
        if (m_shouldCleanup) {
            TotemAnimation::FreeTheMemoryOfTheTotemAnimationClassWhichWouldComeInHandyInCertainCasesLikeWhenTheAnimationIsFinishedAndActuallyItsOnlyPurposeIsToGetFreedWhenTheAnimationIsDoneWhichIsPrettyCool();
        }
        
    }

    auto spriteName = fmt::format("mctotem_anim_{}.png"_spr, frame);

    auto sprite = CCSprite::createWithSpriteFrameName(spriteName.c_str());
    if (sprite) {
        auto sprframe = CCSpriteFrameCache::get()->spriteFrameByName(spriteName.c_str());

        this->setDisplayFrame(sprframe);
    }

    ++frame;
    
}

TotemAnimation* TotemAnimation::create(std::function<void()> onceFinished, bool shouldCleanup) {
    auto ret = new TotemAnimation();
    if (ret->init(onceFinished, shouldCleanup)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}