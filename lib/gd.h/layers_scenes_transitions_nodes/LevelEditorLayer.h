#ifndef __LEVELEDITORLAYER_H__
#define __LEVELEDITORLAYER_H__

#include <gd.h>

namespace gd {

    class GJBaseGameLayer;
    class EditorUI;
    class GameObject;

    class LevelEditorLayer : public GJBaseGameLayer {
    public:
        PAD(0x84)
        int m_currentLayer;
        PAD(0x2c)
        EditorUI* m_editorUI;
        PAD(0x4)
        cocos2d::CCArray* m_undoObjects;

        void removeObject(GameObject * obj, bool idk) {
            reinterpret_cast<void(__thiscall*)(
                LevelEditorLayer *,GameObject *,bool
            )>(
                base + 0x161cb0
            )(
                this, obj, idk
            );
        }

        int getNextFreeGroupID(cocos2d::CCArray* objs) {
            return reinterpret_cast<int(__thiscall*)(
                LevelEditorLayer*, cocos2d::CCArray*
            )>(
                base + 0x164ae0
            )(
                this, objs
            );
        }

        GameObject* addObjectFromString(std::string const& str) {
            return reinterpret_cast<GameObject*(__thiscall*)(LevelEditorLayer*, std::string)>(base + 0x160c80)(this, str);
        }
    };

    enum class UndoCommand {
        Delete = 1,
        New = 2,
        Paste = 3,
        DeleteMulti = 4,
        Transform = 5,
        Select = 6,
    };

    class UndoObject : public cocos2d::CCObject {
    public:
        static UndoObject* createWithArray(cocos2d::CCArray* arr, UndoCommand command) {
            return reinterpret_cast<UndoObject*(__fastcall*)(cocos2d::CCArray*, UndoCommand)>(base + 0x16bee0)(arr, command);
        }

        GameObject* m_singleObject;
        UndoCommand m_command;
        cocos2d::CCArray* m_objects;
        bool m_redo;
    };
}

#endif
