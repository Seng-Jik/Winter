#ifndef _HEAD_SETTING_UI_HEAD_
#define _HEAD_SETTING_UI_HEAD_

#include "Core.h"
#include "ACGCross/Slider.h"
#include "ACGCross/Button.h"

namespace ACGCross{

class SettingUI : public Core::Activity
{
    public:
        SettingUI();
        ~SettingUI();

        void OnShow();
        void OnHide();

        void OnDraw();
        void OnNext();

        void OnEvent(Core::Control*,const Uint32);
        void OnEvent(const SDL_Event&);

    protected:
    private:
        Core::Surface m_bg;
        Core::Texture m_bgt_o;
        Core::Texture m_bgt;

        Button m_return;
        Button m_title;

        void updateGrpState();
        Core::Font m_font;

        Core::Texture m_bgmLab,
                            m_cvLab,
                            m_seLab;
        Slider m_bgmSetting;
        Slider m_seSetting;
        Slider m_cvSetting;


        Core::Timer m_timer;

        enum {SHOWING,HIDING,NOR,TITLING} m_stat = NOR;

        bool m_grpWarn = false;
        Core::Texture m_grpWarn_t;
        Core::Texture m_grp_title;
        Core::Texture m_grp_list[12];
};
}

#endif // _HEAD_SETTING_UI_HEAD_
