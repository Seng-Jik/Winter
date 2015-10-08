#pragma once

#include "Snow.h"
#include "../ACGCross/PAW01_Winter/MathFunc.h"

namespace ACGCross{

    class Logo:public ::Snow::Activity
    {
    private:
        ::Snow::Sprite m_logo;
        ::Snow::Sprite m_logo2;
        ::Snow::Sprite m_yzsz_yu;
        ::Snow::Sprite m_yzsz_zhai;
        ::Snow::Sprite m_yzsz_zi;
        ::Snow::Sprite m_yzsz_eng;
        ::Snow::Sprite m_effect;
        ::Snow::Thread* m_init;
        ::Snow::Timer m_timer;
        Uint8 m_stat;
        ::Snow::Activity* m_goto;

        SDL_Rect m_sta0_logoRect;
        SDL_Rect m_sta1_logoRect;
        SDL_Rect m_sta1_backRect;
    public:
        Logo();

        //以下两接口应该在执行前调用
        void SetInitThread(::Snow::Thread* t){m_init = t;}    //如果需要启动一个线程用于初始化，请在执行前使用这个
        void SetGoto(Activity& a){m_goto = &a;}  //你必须在执行前使用该方法指定要启动的活动

        void OnShow();
        void OnHide();
        void OnNext();
        void OnDraw();
    };

}

