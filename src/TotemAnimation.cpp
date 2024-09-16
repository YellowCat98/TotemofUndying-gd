#include "TotemAnimation.hpp"

using namespace geode::prelude;

bool TotemAnimation::init(std::function<void()> onceFinished) {
    if (!CCSprite::initWithSpriteFrameName("mctotem_anim_01.png"_spr)) return false;

    onFinished = onceFinished;

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
    static int frame = 3;
    if (frame == 67) {
        frame = 3;
        onFinished();
        TotemAnimation::FreeTheMemoryOfTheTotemAnimationClassWhichWouldComeInHandyInCertainCasesLikeWhenTheAnimationIsFinishedAndActuallyItsOnlyPurposeIsToGetFreedWhenTheAnimationIsDoneWhichIsPrettyCool();
    }

    auto spriteName = fmt::format("mctotem_anim_{}.png"_spr, frame);

    auto sprite = CCSprite::createWithSpriteFrameName(spriteName.c_str());
    if (sprite) {
        auto sprframe = CCSpriteFrameCache::get()->spriteFrameByName(spriteName.c_str());

        this->setDisplayFrame(sprframe);
    }

    ++frame;
    
}

TotemAnimation* TotemAnimation::create(std::function<void()> onceFinished) {
    auto ret = new TotemAnimation();
    if (ret->init(onceFinished)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}