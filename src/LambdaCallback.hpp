// quick cccallfunc recreation (copied from overcharged garage (unfinished project))
// i dont like CCCallFunc, feel free to steal this precious piece of code

#pragma once
#include <Geode/Geode.hpp>

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