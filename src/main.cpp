#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

class $modify(CCTextInputNode) {

	bool allowedInput() {
		if (!LevelEditorLayer::get()) return false;
		for (char c : std::string_view(m_allowedChars)) {
			if (!((c >= '0' && c <= '9') || c == '-' || c == '.'))
				return false;
		}
		return true;
	}

    bool init(float p0, float p1, char const* p2, char const* p3, int p4, char const* p5) {
		if (!CCTextInputNode::init(p0, p1, p2, p3, p4, p5)) return false;

		queueInMainThread([self = Ref(this)] {
			if (!self->allowedInput() || !Mod::get()->getSettingValue<bool>("show-negate-button")) return;
			auto menu = CCMenu::create();
			menu->setAnchorPoint({0.f, 0.5f});
			menu->ignoreAnchorPointForPosition(false);
			menu->setPosition((self->getContentSize()/2) - 4);
			menu->setContentSize({12, 12});
			menu->setID("negate-menu"_spr);

			auto spr = CCSprite::createWithSpriteFrameName("edit_delCBtn_001.png");

			auto btnSpr = ButtonSprite::create(spr, 30, 1, 30, 1, false, "GJ_button_06-uhd.png", false);
			btnSpr->setScale(0.3f);

			auto btn = CCMenuItemSpriteExtra::create(btnSpr, self, menu_selector(Self::onNegate));
			btn->setPosition(menu->getContentSize()/2);
			btn->setID("negate-btn"_spr);

			menu->addChild(btn);

			self->addChild(menu);
		});

		return true;
	}

    bool onTextFieldInsertText(cocos2d::CCTextFieldTTF* pSender, char const* text, int nLen, cocos2d::enumKeyCodes keyCodes) {
		if (allowedInput() && Mod::get()->getSettingValue<bool>("enable-negate-keybind")) {
			if (std::string_view(text) == "n") {
				onNegate(nullptr);
				return true;
			}
		}
		return CCTextInputNode::onTextFieldInsertText(pSender, text, nLen, keyCodes);
	}

	void onNegate(CCObject* sender) {
		auto numRes = numFromString<double>(getString());
		if (numRes) {
			auto num = numRes.unwrap();
			num *= -1;
			setString(numToString(num));
		}
	}
};