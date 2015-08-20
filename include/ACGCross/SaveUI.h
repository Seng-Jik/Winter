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
        Core::Texture m_bg;
        Core::Surface m_bgsur;
        Core::Timer m_timer;
        enum state{
            OPENNING,
            SELECTSAVE,
            CLOSING
        }m_stat;
};
}

#endif // _HEAD_SAVEUI_HEAD_
