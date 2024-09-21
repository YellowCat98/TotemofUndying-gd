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

class CoolerDelayTime : public cocos2d::CCDelayTime {
private:
    float m_timeRemaining;
public:
    std::function<void()> whileTimerIsRunning;
    static CoolerDelayTime* create(float duration) {
        CoolerDelayTime* ret = new CoolerDelayTime();
        if (ret->initWithDuration(duration)) {
            ret->autorelease();
            ret->m_timeRemaining = duration;
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }

    virtual void update(float dt) override {
        CCDelayTime::update(dt);
        m_timeRemaining -= dt;
        if (m_timeRemaining > 0.0f) {

            if (whileTimerIsRunning) {
                whileTimerIsRunning();
            }
        } else {
            m_timeRemaining = 0;
        }
    }


    float timeRemaining() const {
        return m_timeRemaining;
    }
};