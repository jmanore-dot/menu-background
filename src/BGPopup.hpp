#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/file.hpp>

class BGPopup : public geode::Popup<> {
protected:
    cocos2d::CCLabelBMFont* m_status = nullptr;
    geode::EventListener<geode::Task<geode::Result<std::filesystem::path>>> m_pickListener;

    bool setup() override;
    void updateStatus();

    void onAdd(cocos2d::CCObject*);
    void onNext(cocos2d::CCObject*);
    void onPrev(cocos2d::CCObject*);
    void onRemove(cocos2d::CCObject*);
    void onDefault(cocos2d::CCObject*);

public:
    static BGPopup* create();
};
