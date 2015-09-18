#include "ACGCross/PAW01_Winter/KeyMapActivity.h"
#include <iostream>
using namespace std;
using namespace ACGCross;

std::map <SDL_Keycode,Uint16> KeyMapActivity::m_keyMap;
std::map <KeyMapActivity::JoyKey,Uint16> KeyMapActivity::m_joyMap;

KeyMapActivity::KeyMapActivity()
{

}

KeyMapActivity::~KeyMapActivity()
{
    //dtor
}

void KeyMapActivity::SetJoyKeyMap(const SDL_JoystickID i, const Uint8 button, Uint16 mappedKey)
{
    JoyKey j = {i,button};
    m_joyMap[j] = mappedKey;
}

void KeyMapActivity::SetKeyboardMap(const SDL_Keycode c, Uint16 mappedKey)
{
    m_keyMap[c] = mappedKey;
}


void KeyMapActivity::OnEvent(const SDL_Event& e)
{
    KeyEvent ke;
    if(e.type == SDL_KEYDOWN || e.type == SDL_JOYBUTTONDOWN)
        ke = KEYDOWN;
    else if(e.type == SDL_KEYUP || e.type == SDL_JOYBUTTONUP)
        ke = KEYUP;

    if(e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
        if(m_keyMap.count(e.key.keysym.sym)) OnKeyEvent(ke,m_keyMap[e.key.keysym.sym]);

    if(e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP){
        JoyKey k= {e.jbutton.which,e.jbutton.button};
        if(m_joyMap.count(k)) OnKeyEvent(ke,m_joyMap[k]);
    }else return;
}

