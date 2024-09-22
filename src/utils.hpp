#pragma once
#include <Geode/Geode.hpp>

// quick cccallfunc recreation (copied from overcharged garage (unfinished project))
// i dont like CCCallFunc, feel free to steal this precious piece of code

class LambdaCallback : public cocos2d::CCActionInstant {
public:
    std::function<void()> callback;

    static LambdaCallback* create(const std::function<void()>& func) {
        auto ret = new LambdaCallback();
        if (ret && ret->initWithCallBack(func)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    bool initWithCallBack(const std::function<void()>& func) {
        callback = func;
        return true;
    }

    virtual void update(float dt) override {
        if (callback) {
            callback();
        }
    }
};

class CoolerDelayTime : public cocos2d::CCActionInterval {
private:
    float m_timeRemaining;

    bool initWithDuration(float duration, std::function<void()> onInit) {
        if (!CCActionInterval::initWithDuration(duration)) return false;
        onInit();
        return true;
    }

public:
    std::function<void(float, float)> whileTimerIsRunning;
    static CoolerDelayTime* create(float duration, std::function<void()> onInit) {
        CoolerDelayTime* ret = new CoolerDelayTime();
        if (ret->initWithDuration(duration, onInit)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    virtual void update(float dt) override {
        // Call the base class update
        CCActionInterval::update(dt);

        m_timeRemaining = dt * m_fDuration;

        m_timeRemaining = m_fDuration - m_timeRemaining;

        whileTimerIsRunning(dt, m_timeRemaining);
    }


    float timeRemaining() const {
        return m_timeRemaining;
    }
};

float limitDecimal(float value) {
    return std::round(value * 100.0f) / 100.0f;
}