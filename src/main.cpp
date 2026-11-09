#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "BackgroundManager.hpp"
#include "BGPopup.hpp"

using namespace geode::prelude;

class $modify(BGMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        // apply the saved background
        bgm::apply(this);

        // button sprite: use a game icon if available, otherwise a "BG" label
        CCNode* icon = nullptr;
        if (CCSpriteFrameCache::get()->spriteFrameByName("GJ_paintBtn_001.png")) {
            icon = CircleButtonSprite::createWithSpriteFrameName(
                "GJ_paintBtn_001.png", 1.f, CircleBaseColor::Green, CircleBaseSize::Medium
            );
        }
        else {
            auto label = CCLabelBMFont::create("BG", "bigFont.fnt");
            icon = CircleButtonSprite::create(label, CircleBaseColor::Green, CircleBaseSize::Medium);
        }

        auto btn = CCMenuItemSpriteExtra::create(icon, this, menu_selector(BGMenuLayer::onPickBackground));
        btn->setID("change-background-button"_spr);

        if (auto menu = this->getChildByID("bottom-menu")) {
            menu->addChild(btn);
            menu->updateLayout();
        }

        return true;
    }

    void onPickBackground(CCObject*) {
        BGPopup::create()->show();
    }
};
