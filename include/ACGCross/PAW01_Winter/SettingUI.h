#ifndef _HEAD_SETTING_UI_HEAD_
#define _HEAD_SETTING_UI_HEAD_

#include "Snow.h"
#include "ACGCross/PAW01_Winter/Slider.h"
#include "ACGCross/PAW01_Winter/Button.h"

namespace ACGCross{

class SettingUI : public ::Snow::Activity
{
    public:
        SettingUI();
        ~SettingUI();

        void OnShow();
        void OnHide();

        void OnDraw();
        void OnNext();

        void OnEvent(::Snow::Control*,const Sint32);
        void OnEvent(const SDL_Event&);

    protected:
    private:
        ::Snow::Surface m_bg;
        ::Snow::Sprite m_bgt_o;
        ::Snow::Sprite m_bgt;

        Button m_return;
        Button m_title;

        void updateGrpState();
        ::Snow::Font m_font;

        ::Snow::Sprite m_bgmLab,
                            m_cvLab,
                            m_seLab;
        Slider m_bgmSetting;
        Slider m_seSetting;
        Slider m_cvSetting;


        ::Snow::Timer m_timer;

        enum {SHOWING,HIDING,NOR,TITLING} m_stat = NOR;

        bool m_grpWarn = false;
        ::Snow::Sprite m_grpWarn_t;
        ::Snow::Sprite m_grp_title;
        ::Snow::Sprite m_grp_list[12];
};
}

#endif // _HEAD_SETTING_UI_HEAD_
