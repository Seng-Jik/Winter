#pragma once

#include "Core.h"
#include "../ACGCross/PAW01_Winter/MathFunc.h"

namespace ACGCross{

    class Logo:public Core::Activity
    {
    private:
        Core::Texture m_logo;
        Core::Texture m_logo2;
        Core::Texture m_yzsz_yu;
        Core::Texture m_yzsz_zhai;
        Core::Texture m_yzsz_zi;
        Core::Texture m_yzsz_eng;
        Core::Texture m_effect;
        Core::Thread* m_init;
        Core::Timer m_timer;
        Uint8 m_stat;
        Core::Activity* m_goto;

        SDL_Rect m_sta0_logoRect;
        SDL_Rect m_sta1_logoRect;
        SDL_Rect m_sta1_backRect;
    public:
        Logo();

        //以下两接口应该在执行前调用
        void SetInitThread(Core::Thread* t){m_init = t;}    //如果需要启动一个线程用于初始化，请在执行前使用这个
        void SetGoto(Activity& a){m_goto = &a;}  //你必须在执行前使用该方法指定要启动的活动

        void OnShow();
        void OnHide();
        void OnNext();
        void OnDraw();
    };

}

