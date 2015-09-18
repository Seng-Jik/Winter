#include "ACGCross/PAW01_Winter/Title/MovingBG.h"

#define MIN_ALP 128

using namespace std;
using namespace Core;
using namespace ACGCross::Galgame;

void MovingBg::InitRectData()
{
    int w,h;

    SDL_QueryTexture(m_tex,nullptr,nullptr,&w,&h);
    m_dst1 = {m_dst.x,m_dst.y,m_dst.w,0};
    m_src1 = {0,h,w,0};

    m_dst2 = {m_dst.x,m_dst.y,w,h};
    m_src2 = {0,0,w,h};
}

void MovingBg::Init(const string& bg)
{
    m_tex = IMG_LoadTexture(pRnd,bg.c_str());
    SDL_SetTextureAlphaMod(m_tex,0);
    m_fpsTimer = 0;
    m_dst.w = pRnd.GetW();
    m_dst.h = pRnd.GetH();
    m_alpha = 255;
    m_alpUp = false;
}

void MovingBg::Quit()
{
    SDL_DestroyTexture(m_tex);
}

void MovingBg::SetDstRect(int x,int y)
{
    m_dst.x = x;
    m_dst.y = y;

}

void MovingBg::SetFadeTime(Uint16 s)
{
    m_fadeTime = s;
}

void MovingBg::FadeOut()
{
    m_stat = -1;
    m_timer.Reset();
}

void MovingBg::FadeIn()
{
    InitRectData();
    m_stat = 1;
    m_timer.Reset();
}

void MovingBg::OnDraw()
{
    SDL_RenderCopy(pRnd,m_tex,&m_src1,&m_dst1);
    SDL_RenderCopy(pRnd,m_tex,&m_src2,&m_dst2);
}

void MovingBg::OnNext()
{
    m_fpsTimer++;
    if(m_stat == 1){
        Uint16 i = float(m_timer.GetTimer()) / m_fadeTime*255;
        SDL_SetTextureAlphaMod(m_tex,i);
        if(i>=255){
            SDL_SetTextureAlphaMod(m_tex,255);
            m_stat = 0;
        }
    }else if(m_stat == -1){
        Uint16 i = float(m_timer.GetTimer()) / m_fadeTime*255;
        SDL_SetTextureAlphaMod(m_tex,255-i);
        if(i>=255){
            SDL_SetTextureAlphaMod(m_tex,0);
            m_stat = 0;
        }
    }


    if(m_fpsTimer == 2){
        m_fpsTimer = 0;

        if(m_dst1.h == m_dst.h) InitRectData();
        m_dst1.h++;
        m_src1.y--;
        m_src1.h++;

        m_dst2.y++;
        m_dst2.h--;
        m_src2.h--;

        if(m_stat == 0){
            if(m_alpUp){
                m_alpha++;
                if(m_alpha >= 255) m_alpUp = false;
            }else{
                m_alpha--;
                if(m_alpha <= MIN_ALP) m_alpUp = true;
            }
            SDL_SetTextureAlphaMod(m_tex,m_alpha);

        }
    }
}

