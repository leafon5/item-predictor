#include "hookmain.hpp"

#include <matdash/minhook.hpp>


bool activated = false;


void PlayLayer_onQuit(gd::PlayLayer* self) {
    activated = false;
    std::cout << "PlayLayer off"<< std::endl;

    matdash::orig<&PlayLayer_onQuit, matdash::Thiscall>(self);
}

matdash::cc::thiscall<void> PlayLayer_Update(gd::PlayLayer* self, float dt) {
    if(activated) {
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
        
        if((value + counter - 20 == 3) && (value != 3)) {
            plabel2->setString(std::to_string(value2).c_str());
        }
        else plabel2->setString(std::to_string((value + counter + value2) > 19 ? value + counter + value2 - 20 : value + counter + value2).c_str());
    }

    matdash::orig<&PlayLayer_Update>(self, dt);
    return {};
}
void collect_hook(gd::GJBaseGameLayer* self, int itemID, int count) {
    // because apparently playlayer::init is doing it too fast???
    if(!activated) {
            auto label = CCLabelBMFont::create("0", "bigFont.fnt");
            auto label2 = CCLabelBMFont::create("0", "bigFont.fnt");
            auto label3 = CCLabelBMFont::create("0", "bigFont.fnt");

            // item value labels

            label->setPosition({10, 280});
            label->setZOrder(100);
            label->setTag(8345);
            self->addChild(label);

            label2->setPosition({50, 280});
            label2->setZOrder(100);
            label2->setTag(8346);
            self->addChild(label2);

            label3->setPosition({90, 280});
            label3->setZOrder(100);
            label3->setTag(8348);
            self->addChild(label3);

            // collected labels

            auto clabel = CCLabelBMFont::create("0", "bigFont.fnt");
            auto clabel2 = CCLabelBMFont::create("0", "bigFont.fnt");

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

            predictionlabel->setPosition({50, 250});
            predictionlabel->setZOrder(100);
            predictionlabel->setTag(8347);
            self->addChild(predictionlabel);

            // prediction labels

            auto plabel = CCLabelBMFont::create("0", "bigFont.fnt");
            auto plabel2 = CCLabelBMFont::create("0", "bigFont.fnt");

            plabel->setPosition({10, 220});
            plabel->setZOrder(100);
            plabel->setTag(8349);
            self->addChild(plabel);

            plabel2->setPosition({50, 220});
            plabel2->setZOrder(100);
            plabel2->setTag(8350);
            self->addChild(plabel2);

    }
    activated = true;

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
    matdash::add_hook<&collect_hook>(gd::base + 0x111890);
    matdash::add_hook<&PlayLayer_onQuit, matdash::Thiscall>(gd::base + 0x20D810);
    //   matdash::add_hook<&PlayLayer_update_, matdash::Thiscall>(gd::base + 0x2029c0);
    matdash::add_hook<&PlayLayer_Update>(gd::base + 0x2029C0);
    // perhaps?

}