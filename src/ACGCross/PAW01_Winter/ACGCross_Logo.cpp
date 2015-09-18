#include "ACGCross/PAW01_Winter/ACGCross_Logo.h"
#include "ACGCross/PAW01_Winter/Game.h"
#define LOGO_SIZE 90
#define PER_LOGO_WH 1.380487804878049f
#define WORD_SIZE 128
using namespace std;
using namespace Core;
using namespace ACGCross;

Logo::Logo()
{
    m_init = nullptr;
    if(gameData.Get16_9()) SetLogicScreenSize(1024,576);
    else SetLogicScreenSize(1024,768);
}

void Logo::OnShow()
{
    //SetLogicScreenSize(1024,576);
    m_logo.Load("ACGCross\\ACGCross_Logo.png");
    m_logo.SetZoom(int(LOGO_SIZE*2),int(LOGO_SIZE*2*PER_LOGO_WH));
    m_logo2.Load("ACGCross\\ACGCross_Logo2.png");
    m_logo2.SetZoom(int(LOGO_SIZE*2),int(LOGO_SIZE*2*PER_LOGO_WH));
    m_yzsz_yu.Load("ACGCross\\ACGCross_Chn_1.png");
    m_yzsz_yu.SetPos(0x22,-2*WORD_SIZE,-WORD_SIZE/2);
    m_yzsz_zhai.Load("ACGCross\\ACGCross_Chn_2.png");
    m_yzsz_zhai.SetPos(0x22,-WORD_SIZE,-WORD_SIZE/2);
    m_yzsz_zi.Load("ACGCross\\ACGCross_Chn_3.png");
    m_yzsz_zi.SetPos(0x22,WORD_SIZE,-WORD_SIZE/2);
    m_yzsz_eng.Load("ACGCross\\ACGCross_Eng.png");
    m_yzsz_eng.SetPos(0x23,-100,-20);
    m_yzsz_eng.SetAlpha(0);

    m_yzsz_yu.SetAlpha(0);
    m_yzsz_zhai.SetAlpha(0);
    m_yzsz_zi.SetAlpha(0);

    m_logo.SetPos(0x22,-LOGO_SIZE,-LOGO_SIZE*PER_LOGO_WH+15);
    m_logo2.SetPos(0x22,-LOGO_SIZE,-LOGO_SIZE*PER_LOGO_WH+15);
    m_effect.Load("ACGCross\\ACGCross_Effect.png");
    m_effect.SetAlpha(128);
    //m_yzsz.SetAlpha(0);

    if(m_init != nullptr) m_init -> Run();
    m_stat = 0;
    m_logo.SetBlend(SDL_BLENDMODE_MOD);
    m_logo.SetAlpha(0);

    Pos(m_sta0_logoRect.x,m_sta0_logoRect.y,0x22,-192,-192);
    m_sta0_logoRect.w = 0;m_sta0_logoRect.h = 0;

    m_sta1_logoRect = m_sta0_logoRect;
    m_sta1_logoRect.x += 1;m_sta1_logoRect.y += 1;
    m_sta1_logoRect.w = 382;
    m_sta1_backRect = {0,pRnd.GetH(),pRnd.GetW(),0};
    m_timer.Reset();
}

void Logo::OnHide()
{
    m_logo.Clear();
    m_logo2.Clear();
    m_yzsz_yu.Clear();
    m_yzsz_zhai.Clear();
    m_yzsz_zi.Clear();
    m_yzsz_eng.Clear();
    m_effect.Clear();
}

void Logo::OnNext()
{
    if(m_stat == 0){
        m_sta0_logoRect.h = m_sta0_logoRect.w = float(m_timer.GetTimer()) / 256 * 384;
        if(m_sta0_logoRect.h >= 384) m_sta0_logoRect.h = m_sta0_logoRect.w = 384;
    }else if(m_stat == 1){
        float x = ArcFunc(float(m_timer.GetTimer()) / 512);
        m_sta1_logoRect.y = pRnd.GetH()/2 - 192 +382 * x;
        m_sta1_logoRect.h = pRnd.GetH()/2 + 192 - m_sta1_logoRect.y;
        m_sta1_backRect.h = -(pRnd.GetH() * x);
        m_logo2.SetAlpha(x * 255);
        if(x == -1.0) {m_stat = 2;m_timer.Reset();m_logo2.SetAlpha(255);}
    }else if(m_stat == 2){
        float x = ArcFunc(float(m_timer.GetTimer()) / 512);

        int logo2_x,logo2_x_dst,logo2_y;
        Pos(logo2_x,logo2_y,0x22,-LOGO_SIZE,-LOGO_SIZE*PER_LOGO_WH+15);
        Pos(logo2_x_dst,logo2_y,0x22,-25,-LOGO_SIZE*PER_LOGO_WH+15);
        m_logo2.SetPos(int((logo2_x_dst - logo2_x)*x + logo2_x),logo2_y);

        m_yzsz_yu.SetAlpha(255*x);
        m_yzsz_zhai.SetAlpha(255*x);
        m_yzsz_zi.SetAlpha(255*x);
        m_yzsz_eng.SetAlpha(255*x);
        if(x == -1.0){
            m_stat = 3;
            m_logo2.SetPos(logo2_x_dst,logo2_y);
            m_yzsz_yu.SetAlpha(255);
            m_yzsz_zhai.SetAlpha(255);
            m_yzsz_zi.SetAlpha(255);
            m_yzsz_eng.SetAlpha(255);
            m_timer.Reset();
        }
    }
    else if(m_stat == 3)
    {
        float x = ArcFunc(float(m_timer.GetTimer()) / 1024);
        if(x<=1.0)
            m_effect.SetPos(int(pRnd.GetW() * x)-pRnd.GetW()/3 -70,pRnd.GetH()/2-150 - 70);
        else m_effect.SetAlpha(0);
    }
    else if(m_stat == 4){
        float x = ArcFunc(float(m_timer.GetTimer()) / 512);
        Uint8 v = 255*x;
        m_logo2.SetAlpha(255-v);
        m_yzsz_eng.SetAlpha(255-v);
        m_yzsz_yu.SetAlpha(255-v);
        m_yzsz_zhai.SetAlpha(255-v);
        m_yzsz_zi.SetAlpha(255-v);
        //m_yzsz.SetAlpha(255-v);
        if(x == -1.0) {
            m_stat = 5;
        }
    }else if(m_stat == 5){
        Goto(m_goto);
    }
}

void Logo::OnDraw()
{
    if(m_stat == 0){
        SDL_SetRenderDrawColor(pRnd,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderDrawRect(pRnd,&m_sta0_logoRect);
        if(m_sta0_logoRect.h >= 384) {m_stat = 1;m_timer.Reset();}
    }else if(m_stat == 1){
        SDL_SetRenderDrawColor(pRnd,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderFillRect(pRnd,&m_sta0_logoRect);

        SDL_RenderFillRect(pRnd,&m_sta1_backRect);
        SDL_SetRenderDrawColor(pRnd,0x00,0x00,0x00,0xFF);
        m_logo.OnDraw();
        m_logo2.OnDraw();
        SDL_RenderFillRect(pRnd,&m_sta1_logoRect);


    }else if(m_stat == 2 || m_stat == 3){
        SDL_SetRenderDrawColor(pRnd,0xFF,0xFF,0xFF,0xFF);
        pRnd.Clear();
        m_logo2.OnDraw();
        //m_yzsz.OnDraw();
        m_yzsz_yu.OnDraw();
        m_yzsz_zhai.OnDraw();
        m_yzsz_zi.OnDraw();
        m_yzsz_eng.OnDraw();
        m_effect.OnDraw();
        if(m_stat == 3 && m_timer.GetTimer() >= 1500){
            if(m_init != nullptr) {if(!m_init -> Running()) {m_stat = 4;m_timer.Reset();}}
            else {m_stat = 4;m_timer.Reset();}
        }
    }else if(m_stat == 4){
        SDL_SetRenderDrawColor(pRnd,0xFF,0xFF,0xFF,0xFF);
        pRnd.Clear();
        m_logo2.OnDraw();
        m_yzsz_yu.OnDraw();
        m_yzsz_zhai.OnDraw();
        m_yzsz_zi.OnDraw();
        m_yzsz_eng.OnDraw();
        //m_yzsz.OnDraw();
    }else{
        SDL_SetRenderDrawColor(pRnd,255,255,255,0xFF);
        pRnd.Clear();
    }
}
