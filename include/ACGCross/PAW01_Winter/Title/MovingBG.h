#pragma once

#include "Snow.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"

namespace ACGCross{
namespace Galgame{

    class MovingBg
    {
    private:
        SDL_Texture* m_tex;
        SDL_Rect m_src1;
        SDL_Rect m_src2;
        SDL_Rect m_dst1;
        SDL_Rect m_dst2;
        SDL_Rect m_dst;
        Uint16 m_fadeTime;
        int m_stat;
        ::Snow::Timer m_timer;
        int m_FrameTimer;

        Uint8 m_alpha;
        bool m_alpUp;
        void InitRectData();
    public:
        void Init(const std::string& bg);
        void Quit();
        void OnNext();
        void OnDraw();
        void FadeIn();
        void FadeOut();
        void SetFadeTime(Uint16);   //最先执行这两个函数
        void SetDstRect(int x,int y);
    };
}
}
