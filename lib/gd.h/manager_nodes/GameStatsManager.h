#pragma once

#include <cocos2d.h>

namespace gd {

class GameStatsManager : public cocos2d::CCNode {
public:
    static GameStatsManager* sharedState() {
        return reinterpret_cast<GameStatsManager*(__stdcall*)()>(base + 0xf1e50)();
    }

    int getStat(const char* key) {
        return reinterpret_cast<int(__thiscall*)(GameStatsManager*, const char*)>(base + 0xf3580)(this, key);
    }
};

}