#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

CCSpriteFrame *updateSpriteCache(int pride = Mod::get()->getSettingValue<int64_t>("pride")) {
    auto cache = CCSpriteFrameCache::get();
	auto wokeFrame = cache->spriteFrameByName(fmt::format("{}.png"_spr, pride).c_str());
	cache->removeSpriteFrameByName("GJ_logo_001.png");
	cache->addSpriteFrame(wokeFrame, "GJ_logo_001.png");
    return wokeFrame;
}

class $modify(MyLoadingLayer, LoadingLayer) {
    bool init(bool p0) {
        CCFileUtils::sharedFileUtils()->addSearchPath((Mod::get()->getTempDir() / "resources").string().c_str());
        CCSpriteFrameCache::get()->addSpriteFramesWithFile("WokeSheet.plist"_spr);
        if (!LoadingLayer::init(p0)) return false;
        getChildByType<CCSprite*>(1)->setDisplayFrame(updateSpriteCache());
        return true;
    }
};

class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		auto logo = this->getChildByIDRecursive("main-title");
		if (!logo) return true;
		auto pos = logo->getPosition();
        logo->removeFromParentAndCleanup(false);

		auto btn = CCMenuItemSpriteExtra::create(logo, this, menu_selector(MyMenuLayer::openGeometryPrideSettings));
		btn->m_scaleMultiplier = 1.075f;
        btn->setID("woke-title"_spr);
		auto menu = CCMenu::create();
        menu->setID("woke-menu"_spr);
		menu->addChild(btn);
		btn->setPosition(pos);
		menu->setPosition({0, 0});
		this->addChild(menu);

        return true;    
	}

	void openGeometryPrideSettings(CCObject* sender) {
		openSettingsPopup(Mod::get());
	}
};

$on_mod(Loaded) {
    geode::listenForSettingChanges<uint64_t>("pride", [](uint64_t val) {
        auto frame = updateSpriteCache(val);
        if (auto ml = MenuLayer::get()) {
            if (auto logo = typeinfo_cast<CCSprite *>(ml->getChildByIDRecursive("main-title"))) {
                logo->setDisplayFrame(frame);
            }
        }
    });
}