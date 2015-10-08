#ifndef _HEAD_GALSELBUTTON_HEAD_
#define _HEAD_GALSELBUTTON_HEAD_

#include "Snow.h"
#include "ACGCross/PAW01_Winter/Game.h"

namespace ACGCross{
namespace Galgame{
class GalSelButton : public ::Snow::Control
{
    public:
        GalSelButton();
        void SetText(const std::wstring&);
        void Show(int);
        void Hide();
        void OnNext();
        void OnDraw();
        bool OnEvent(const SDL_Event&,::Snow::Activity& a);
        void SetY(int y);
        bool Killed();
    protected:
    private:
        friend void ACGCross::InitGame();
        ::Snow::Sprite m_str;
        static SDL_Texture* m_btnbk [3];
        SDL_Rect m_src,m_dst;
        int m_bkW;
        int m_sNum;
        ::Snow::Timer m_tmr;
        enum{SHOWING,HIDING,NORMAL}m_state;
        int m_mouseState = 0;
        bool m_killed = false;
};
}
}

#endif // _HEAD_GALSELBUTTON_HEAD_
