#ifndef KEYMAPACTIVITY_H
#define KEYMAPACTIVITY_H

#include "Core/Activity.h"
#include <map>
#include <vector>

namespace ACGCross{
/* 按键事件映射活动基类 */
//把SDL的按键事件转换为游戏内特定的事件
//比如“按下K”转换为“跳跃”
//使用SDL_GetKeyName作为判断基准
//事件会被转换为Uint16，请将特殊按键定义为宏
//所有该类子类对象都会使用同一份按键映射

class KeyMapActivity : public Core::Activity
{
    public:
        enum KeyEvent{
            KEYDOWN = 1,KEYUP = 0
        };

        static void SetKeyboardMap(const SDL_Keycode,Uint16 mappedKey);
        static void SetJoyKeyMap(const SDL_JoystickID,const Uint8 button,Uint16 mappedKey);

        KeyMapActivity();
        virtual ~KeyMapActivity();
        void OnEvent(const SDL_Event&); //重写此函数时应当先调用KeyMapActivity::OnEvent(const SDL_Event&)

        //重写此函数以获得映射后的按键事件
        virtual void OnKeyEvent(KeyEvent,Uint16) = 0;
    protected:
    private:
        struct JoyKey{
            SDL_JoystickID jid;
            Uint8 btn;
            bool operator < (const JoyKey & cmp) const
            {return btn < cmp.btn;}
        };
        static std::map <SDL_Keycode,Uint16> m_keyMap;
        static std::map <JoyKey,Uint16> m_joyMap;
};
}

#endif // KEYMAPACTIVITY_H
