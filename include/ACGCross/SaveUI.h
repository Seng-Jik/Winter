#ifndef _HEAD_SAVEUI_HEAD_
#define _HEAD_SAVEUI_HEAD_

#include "Core.h"

namespace ACGCross{

class SaveUI : public Core::Activity
{
    public:
        SaveUI();
        void OnShow();
        void OnHide();
        void OnNext();
        void OnDraw();
        void OnEvent(const SDL_Event& e);
        void OnEvent(Core::Control*,const Uint32);
    protected:
    private:
        Core::Surface m_bg;
        Core::Texture m_bgt_o;
        Core::Texture m_bgt;

        Core::Timer m_timer;
        enum {SHOWING,HIDING,NOR,TITLING} m_stat = NOR;
};
}

#endif // _HEAD_SAVEUI_HEAD_
