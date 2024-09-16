#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "TotemAnimation.hpp"
#include <Geode/modify/GameStatsManager.hpp>

using namespace geode::prelude;
using namespace keybinds;

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

				auto totem = TotemAnimation::create([=]() {
					m_fields->m_shouldNoclip = false;
				}, true);

				totem->setPosition({winSize.width / 2, winSize.height / 2});

				this->addChild(totem);

			}
			
			return ListenerResult::Propagate;
		}, "totem-keybind"_spr);

		// setup mobile ui, can be disabled on android only

		if (Mod::get()->getSettingValue<bool>("disable-button")) {
			if (auto pauseMenu = this->getChildByIDRecursive("pause-button-menu")) {
				auto btnSpr = CCSprite::create("activate_totem.png"_spr);
				btnSpr->setOpacity(75);
				//auto btn = CCMenuItemSpriteExtra::create(btnSpr, this, CCMenuItemExt::LambdaCallback::create([=]() {

				//}));

				auto btn = CCMenuItemExt::createSpriteExtra(btnSpr, [=](CCObject* sender) {
					InvokeBindEvent("totem-keybind"_spr, true).post();
				});
				btn->setID("button-totem"_spr);

				auto layout = RowLayout::create()
					->setAxis(Axis::Row)
					->setCrossAxisAlignment(AxisAlignment::Center);

				
				pauseMenu->setLayout(layout, true, true);

				pauseMenu->addChild(btn);

				pauseMenu->setPositionX(pauseMenu->getPositionX() - 15);
				pauseMenu->updateLayout();
			}
		}
		return true;
	}
	void destroyPlayer(PlayerObject* p0, GameObject* p1) {
		if (p1 == m_anticheatSpike) return PlayLayer::destroyPlayer(p0, p1);

		if (!m_fields->m_shouldNoclip) {
			PlayLayer::destroyPlayer(p0, p1);
		}
	}
};