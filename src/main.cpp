#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "TotemAnimation.hpp"
#include "utils.hpp"
#include <Geode/modify/GameStatsManager.hpp>
#include <Geode/modify/EndLevelLayer.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>

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

	#ifdef GEODE_IS_MOBILE
	listenForSettingChanges("show-button", +[](bool value) {
		if (!value) {
			Mod::get()->setSettingValue<bool>("show-button", true);
		}
	});
	#endif
}

class $modify(GameStatsManagerHook, GameStatsManager) {
struct Fields {
	int m_starsToShow;
};
	void incrementStat(const char* p0, int p1) {
		GameStatsManager::incrementStat(p0, p1);
		if (strcmp(p0, "6") == 0) { // why.
			m_fields->m_starsToShow = p1;
			if (int totemCount = Mod::get()->getSavedValue<int64_t>("totem-count")) {
				Mod::get()->setSavedValue<int64_t>("totem-count", p1 + totemCount);
			} else {
				Mod::get()->setSavedValue<int64_t>("totem-count", p1);
			}
		}
	}
};

bool hasSufficientTotems() {
	return Mod::get()->getSavedValue<int64_t>("totem-count") > 0;
}

class $modify(PlayLayerHook, PlayLayer) {
struct Fields {
	bool m_shouldNoclip;
	//CCParticleSystemQuad* particles;
	bool m_shouldShowIndicator;
	bool m_canActivateTotem = true; // always assume player can activate totem when layer is entered
};

	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
			if (event->isDown()) {
				if (hasSufficientTotems() || m_fields->m_canActivateTotem) {
					Mod::get()->setSavedValue<int64_t>("totem-count", Mod::get()->getSavedValue<int64_t>("totem-count") - 1);
					m_fields->m_shouldNoclip = true;
					m_fields->m_shouldShowIndicator = true;
					m_fields->m_canActivateTotem = false;
					auto winSize = CCDirector::sharedDirector()->getWinSize();

					auto timeElapsedLabel = CCLabelBMFont::create("", "bigFont.fnt");
					timeElapsedLabel->setPosition(ccp(winSize.width - timeElapsedLabel->getContentSize().width / 2, timeElapsedLabel->getContentSize().height / 2));
					timeElapsedLabel->setAnchorPoint(ccp(1.0f, 0.0f));
					timeElapsedLabel->setScale(0.5f);
					this->addChild(timeElapsedLabel);


					// start the timer
					auto delay = CoolerDelayTime::create(2.0f);
					delay->whileTimerIsRunning = [=](float dt) {
						timeElapsedLabel->setString(fmt::format("{}s", std::round(delay->timeRemaining() * 100.0f) / 100.0f).c_str());
					};

					auto callback = LambdaCallback::create([=]() {
						m_fields->m_canActivateTotem = true;
						timeElapsedLabel->setString("");
					});

					auto sequence = CCSequence::create(delay, callback, nullptr);

					this->runAction(sequence);
					

					
					

					auto totem = TotemAnimation::create([=]() {
						m_fields->m_shouldNoclip = false;

						//if (m_fields->particles) {
						//	m_fields->particles->stopSystem();

						//	m_fields->particles->removeFromParent();
						//}
					}, true);

					totem->setPosition({winSize.width / 2, winSize.height / 2});

					this->addChild(totem);
				} else {
					GJBaseGameLayer::get()->shakeCamera(0.2f, 5.0f, 0.0f);
				}
			}

			return ListenerResult::Propagate;
		}, "totem-keybind"_spr);

		if (Mod::get()->getSettingValue<bool>("show-button")) {
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
		} else {
			//m_fields->particles = CCParticleSystemQuad::create("particle_effect.plist"_spr, false);
			//particles->setID("particles");
			//p0->addChild(m_fields->particles);
		}
	}
	void resetLevel() {
		PlayLayer::resetLevel();
		m_fields->m_shouldShowIndicator = false;
	}
};

class $modify(EndLevelLayerHook, EndLevelLayer) {
	void customSetup() {
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		EndLevelLayer::customSetup();

		if (static_cast<PlayLayerHook*>(m_playLayer)->m_fields->m_shouldShowIndicator) {
			auto spr = CCSprite::create("totem.png"_spr);
			spr->setOpacity(0);

			auto sprSize = spr->getContentSize();
			spr->setPosition({sprSize.width / 2, winSize.height - sprSize.height / 2});
			spr->setID("totem-indicator"_spr);

			this->addChild(spr);

			auto fadeIn = CCFadeIn::create(1.0f);

			spr->runAction(fadeIn);
		}
		auto gsm = static_cast<GameStatsManagerHook*>(GameStatsManager::sharedState());
		
		if (gsm->m_fields->m_starsToShow > 0) {
			auto label = CCLabelBMFont::create(fmt::format("+{}", gsm->m_fields->m_starsToShow).c_str(), "bigFont.fnt");
			label->setPosition({140.0f, 175.0f});
			label->setScale(0.850f);
			label->setID("totem-earned-label"_spr);
			

			auto totem = CCSprite::create("totem.png"_spr);

			totem->setPosition({label->getPositionX() + 40, label->getPositionY()});
			totem->setID("totem"_spr);

			m_mainLayer->addChild(totem);
			m_mainLayer->addChild(label);
		}
	}
};