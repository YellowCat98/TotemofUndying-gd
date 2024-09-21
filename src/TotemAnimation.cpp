#include "TotemAnimation.hpp"

using namespace geode::prelude;

bool TotemAnimation::init(std::function<void()> onceFinished, bool shouldCleanup) {
	if (!CCSprite::init()) return false;
	this->schedule(schedule_selector(TotemAnimation::startAnimation), (0.3f/12.0f));
	this->setScale(2.850f); // set scale cause 1 looks small
	this->setID("totem-animation"_spr);

	onFinished = onceFinished;
	m_shouldCleanup = shouldCleanup;

	auto fmod = FMODAudioEngine::sharedEngine();
	fmod->playEffect("totemfx.mp3"_spr);

	
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

	auto spriteName = fmt::format("mctotem_anim_{:02d}.png"_spr, frame);

	auto sprframe = CCSpriteFrameCache::get()->spriteFrameByName(spriteName.c_str());
	if (sprframe) {
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