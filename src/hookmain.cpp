#include "hookmain.hpp"
#include "HitboxNode.hpp"

#include <matdash/minhook.hpp>


bool activated = false;
bool inPlayLayer = false;
std::vector<int> mojGroups = {
    259,
    250,
    253,
    254,
    255,
    264,
    260,
    256,
    257,
    252,
    251,
    262,
    263,
    265,
    247,
    248,
    249,
    246,
    245,
    243
};


int sectionForPos(float x) {
    int section = x / 100;
    if (section < 0)
        section = 0;
    return section;
}

HitboxNode* drawthingy;

void PlayLayer_onQuit(gd::PlayLayer* self) {
    activated = false;
    if(drawthingy) drawthingy->clear();
    inPlayLayer = false;
    std::cout << "PlayLayer off"<< std::endl;
    matdash::orig<&PlayLayer_onQuit, matdash::Thiscall>(self);
}
bool LevelEditorLayer_init(gd::LevelEditorLayer* self, gd::GJGameLevel* level) {
    if(!matdash::orig<&LevelEditorLayer_init>(self, level)) return false;
    activated = false;
    if(drawthingy) drawthingy->clear();
    inPlayLayer = false;
    std::cout << "PlayLayer off" << std::endl;
    return true;
}
bool PlayLayer_init(gd::PlayLayer* self, gd::GJGameLevel* level) {
    if (!matdash::orig<&PlayLayer_init>(self, level)) return false;
    inPlayLayer = true;
    std::cout << "PlayLayer on" << std::endl;
    return true;
}

matdash::cc::thiscall<void> PlayLayer_Update(gd::PlayLayer* self, float dt) {
    if(!activated) {
            matdash::orig<&PlayLayer_Update>(self, dt);
            return {};
    };
    float xp = self->m_player1->getPositionX();

    int value = self->m_effectManager->m_itemValues[1];
    int value2 = self->m_effectManager->m_itemValues[2];
    int counter = self->m_effectManager->m_itemValues[0] + 1;

    // i apologise, i CANNOT figure out a way to do this without getting it every frame, help me my brain is not able to handle this

    auto label = (CCLabelBMFont *) self->getChildByTag(8345);
    auto label2 = (CCLabelBMFont *) self->getChildByTag(8346);
    auto counterlabel = (CCLabelBMFont *) self->getChildByTag(8348);

    auto plabel = (CCLabelBMFont *) self->getChildByTag(8349);
    auto plabel2 = (CCLabelBMFont *) self->getChildByTag(8350);

    auto clabel = (CCLabelBMFont *) self->getChildByTag(8351);
    auto clabel2 = (CCLabelBMFont *) self->getChildByTag(8352);

    
    label->setString(std::to_string(value).c_str());
    label2->setString(std::to_string(value2).c_str());
    counterlabel->setString(std::to_string(counter).c_str());

    if(value2 + counter + value > 39) value2 -= 20;

    // EXPLANATION OF THIS MESS: if the sums are above 19, subtract 20 from them.

    plabel->setString(std::to_string((value + counter > 19 ? value + counter - 20 : value + counter)).c_str());
    
    int predictionvalue2 = (value + counter + value2) > 19 ? value + counter + value2 - 20 : value + counter + value2;
    int predictionvalue = value + counter > 19 ? value + counter - 20 : value + counter;
    
    // exceptions:

    if((value + counter - 20 == 3) && (value != 3)) {
        predictionvalue2 = value2;
    }
    else if((value + counter - 20 == 2) && (value != 2)) {
        predictionvalue2 = value2;
    }

    if(predictionvalue2 < 0) {
        predictionvalue2 = value2;
    }

    plabel2->setString(std::to_string(predictionvalue2).c_str());
    if(drawthingy)
	    drawthingy->clear();

		for (int s = sectionForPos(xp) - 5; s < sectionForPos(xp) + 6; ++s)
		{
			if (s < 0)
				continue;
			if (s >= self->m_sectionObjects->count())
				break;
			auto section = static_cast<CCArray*>(self->m_sectionObjects->objectAtIndex(s));
			for (size_t i = 0; i < section->count(); ++i)
			{
				auto obj = static_cast<gd::GameObject*>(section->objectAtIndex(i));

				if (obj->m_nObjectID != 749 && obj->getObjType() == gd::GameObjectType::kGameObjectTypeDecoration)
					continue;
                for (int i = 0; i < static_cast<int>(obj->m_groupCount); i++) {
                    if(static_cast<int>(obj->m_groups[i]) == mojGroups[predictionvalue2]) {
        				drawthingy->drawForObject(obj);
                    }
                }
			}
		}

    matdash::orig<&PlayLayer_Update>(self, dt);
    return {};
}
void collect_hook(gd::GJBaseGameLayer* self, int itemID, int count) {
    // because apparently playlayer::init is doing it too fast???
    if(!inPlayLayer) {
        matdash::orig<&collect_hook>(self, itemID, count);
        return;
    }
    if(!activated) {
            
            auto label = CCLabelBMFont::create("0", "bigFont.fnt");
            auto label2 = CCLabelBMFont::create("0", "bigFont.fnt");
            auto label3 = CCLabelBMFont::create("0", "bigFont.fnt");

            // item value labels
            label->setScale(0.5);
            label->setPosition({10, 280});
            label->setZOrder(100);
            label->setTag(8345);
            self->addChild(label);

            label2->setScale(0.5);
            label2->setPosition({50, 280});
            label2->setZOrder(100);
            label2->setTag(8346);
            self->addChild(label2);

            label3->setScale(0.5);
            label3->setPosition({90, 280});
            label3->setZOrder(100);
            label3->setTag(8348);
            self->addChild(label3);

            // collected labels

            auto clabel = CCLabelBMFont::create("0", "bigFont.fnt");
            auto clabel2 = CCLabelBMFont::create("0", "bigFont.fnt");
            clabel->setScale(0.5);
            clabel2->setScale(0.5);

            clabel->setPosition({10, 300});
            clabel->setZOrder(100);
            clabel->setTag(8351);
            clabel->setOpacity((GLubyte)100);

            self->addChild(clabel);

            clabel2->setPosition({50, 300});
            clabel2->setZOrder(100);
            clabel2->setTag(8352);
            clabel2->setOpacity((GLubyte)100);
            self->addChild(clabel2);

            // prediction text

            auto predictionlabel = CCLabelBMFont::create("PREDICTION:", "bigFont.fnt");
            predictionlabel->setScale(0.5);

            predictionlabel->setPosition({55, 250});
            predictionlabel->setZOrder(100);
            predictionlabel->setTag(8347);
            self->addChild(predictionlabel);

            // prediction labels

            auto plabel = CCLabelBMFont::create("0", "bigFont.fnt");
            auto plabel2 = CCLabelBMFont::create("0", "bigFont.fnt");
            plabel->setScale(0.5);
            plabel2->setScale(0.5);

            plabel->setPosition({10, 220});
            plabel->setZOrder(100);
            plabel->setTag(8349);
            self->addChild(plabel);

            plabel2->setPosition({50, 220});
            plabel2->setZOrder(100);
            plabel2->setTag(8350);
            self->addChild(plabel2);
            
            drawthingy = HitboxNode::getInstance();
    

            if(drawthingy) self->m_objectLayer->addChild(drawthingy, 999);
    
            drawthingy->setVisible(true);

            activated = true;
    }

    if(itemID != 0) {
        if(count != -20) {
            auto clabel = (CCLabelBMFont *) self->getChildByTag(8351);
            auto clabel2 = (CCLabelBMFont *) self->getChildByTag(8352);
            if(itemID == 1) clabel->setString(std::to_string(count).c_str());
            if(itemID == 2) clabel2->setString(std::to_string(count).c_str());
        }
    }
    matdash::orig<&collect_hook>(self, itemID, count);
}
auto cocos(const char* symbol) {
    static auto mod = GetModuleHandleA("libcocos2d.dll");
    return GetProcAddress(mod, symbol);
}

void Hooks::init() {
    matdash::add_hook<&PlayLayer_init>(gd::base + 0x1FB780);
    matdash::add_hook<&collect_hook>(gd::base + 0x111890);
    matdash::add_hook<&PlayLayer_onQuit, matdash::Thiscall>(gd::base + 0x20D810);
    matdash::add_hook<&LevelEditorLayer_init>(gd::base + 0x15EE00);
    //   matdash::add_hook<&PlayLayer_update_, matdash::Thiscall>(gd::base + 0x2029c0);
    matdash::add_hook<&PlayLayer_Update>(gd::base + 0x2029C0);
    // perhaps?

}