#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "lambdaCallback.hpp"
#include "TotemAnimation.hpp"

using namespace geode::prelude;
using namespace keybinds;

class $modify(MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto sprite = TotemAnimation::create([=]() {
			log::info("Animation is done!");
		});

		sprite->setPosition({winSize.width / 2, winSize.height / 2});

		this->addChild(sprite);
		return true;
	}
};

$execute {
	BindManager::get()->registerBindable({
		"totem-keybind"_spr,
		"Totem of Undying keybind",

		"",

		{ Keybind::create(KEY_C, Modifier::None) },

		Mod::get()->getName()
	});
}

class $modify(PlayLayerHook, PlayLayer) {
struct Fields {
	bool m_shouldNoclip;
};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
			if (event->isDown()) {
				m_fields->m_shouldNoclip = true;
				auto winSize = CCDirector::sharedDirector()->getWinSize();

				auto totem = TotemAnimation::create([&]() {
					m_fields->m_shouldNoclip = false;
				});

				totem->setPosition({winSize.width / 2, winSize.height / 2});

				this->addChild(totem);

			}
			
			return ListenerResult::Propagate;
		}, "totem-keybind"_spr);
		return true;
	}
	void destroyPlayer(PlayerObject* p0, GameObject* p1) {
		if (p1 == m_anticheatSpike) return PlayLayer::destroyPlayer(p0, p1);

		if (!m_fields->m_shouldNoclip) {
			PlayLayer::destroyPlayer(p0, p1);
		}
		
	}
};
/*
class $modify(GJBaseGameLayerHook, GJBaseGameLayer) {
	void resetPlayer() {
		GJBaseGameLayer::resetPlayer();
		if (auto playLayer = PlayLayer::get()) {
			static_cast<PlayLayerHook*>(playLayer)->m_fields->m_isDead = false;
		}
	}
};

class $modify(PlayerObjectHook, PlayerObject) {
	void pushButton(PlayerButton p0) {
		log::info("is this beiong called whendead");

		if (auto playLayer = PlayLayer::get()) {
			if (static_cast<PlayLayerHook*>(playLayer)->m_fields->m_isDead) {
				auto sprite = CCSprite::create("totem.png"_spr);
				this->addChild(sprite);
			}
		}
		PlayerObject::pushButton(p0);
	}
};
*/