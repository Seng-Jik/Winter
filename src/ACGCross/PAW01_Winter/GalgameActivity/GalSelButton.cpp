#include "ACGCross/PAW01_Winter/GalgameActivity/GalSelButton.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalgameActivity.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"
#define FADE_TIME 500
#define STRF_TIME 200
using namespace std;
using namespace ::Snow;
using namespace ACGCross::Galgame;

SDL_Texture* GalSelButton::m_btnbk[3] = {nullptr};

GalSelButton::GalSelButton()
{
    SDL_QueryTexture(m_btnbk[0],nullptr,nullptr,&m_bkW,&m_dst.h);
    m_src.w = m_dst.w = 0;
    m_src.h = m_dst.h;
    m_src.x = m_src.y = 0;
    m_dst.x = pRnd.GetW()/2 - m_bkW/2;
}

void GalSelButton::SetText(const std::wstring& s)
{
    m_str.Load(((GalgameActivity*)pGal) -> m_name_font,s);
    m_str.SetAlpha(0);
}

void GalSelButton::Show(int num)
{
    m_sNum = num;
    m_tmr.Reset();
    m_state = SHOWING;
}

void GalSelButton::Hide()
{
    m_tmr.Reset();
    m_state = HIDING;
}

void GalSelButton::OnNext()
{
    if(m_state == SHOWING){
        if(m_tmr.GetTimer() >= Uint32(FADE_TIME+STRF_TIME+50*m_sNum)){
            m_str.SetAlpha(255);
            m_src.w = m_bkW;
            m_state = NORMAL;
        }else if(m_tmr.GetTimer() >= (Uint32)FADE_TIME && m_tmr.GetTimer() <= Uint32(FADE_TIME + STRF_TIME+50*m_sNum)){
            m_dst.w = m_src.w = m_bkW;
            m_str.SetAlpha(255* float(m_tmr.GetTimer() - FADE_TIME-50*m_sNum)/(STRF_TIME));
        }else if(m_tmr.GetTimer() < Uint32(FADE_TIME+50*m_sNum)){
            m_dst.w = m_src.w = m_bkW * ACGCross::ArcFunc(float(m_tmr.GetTimer()-50*m_sNum) / FADE_TIME);
        }
    }

    if(m_state == HIDING){
        if(m_tmr.GetTimer() >= Uint32(FADE_TIME+STRF_TIME+50*m_sNum)){
            m_str.SetAlpha(255);
            m_src.w = m_bkW;
            m_state = NORMAL;
            m_killed = true;
        }else if(m_tmr.GetTimer() >= Uint32(STRF_TIME+50*m_sNum) && m_tmr.GetTimer() <= Uint32(FADE_TIME + STRF_TIME+50*m_sNum)){
            m_dst.w = m_src.w = m_bkW * (1 - ACGCross::ArcFunc(float(m_tmr.GetTimer()-STRF_TIME-50*m_sNum) / FADE_TIME));
        }else if(m_tmr.GetTimer() < Uint32(STRF_TIME+50*m_sNum)){
            m_str.SetAlpha(255 - 255* float(m_tmr.GetTimer())/(STRF_TIME+50*m_sNum));
        }
    }
}

void GalSelButton::OnDraw()
{
    SDL_RenderCopy(pRnd,m_btnbk[m_mouseState],&m_src,&m_dst);
    m_str.OnDraw();
}

bool GalSelButton::OnEvent(const SDL_Event& e, Activity& a)
{
    if(e.type == SDL_MOUSEMOTION){
        if(InRect(m_dst,e.motion.x,e.motion.y)) m_mouseState =1;
        else m_mouseState =0;
        return false;
    }else if(e.type == SDL_MOUSEBUTTONDOWN){
        if(InRect(m_dst,e.motion.x,e.motion.y)) m_mouseState =2;
        else m_mouseState =0;
        return false;
    }else if(e.type == SDL_MOUSEBUTTONUP){
        if(InRect(m_dst,e.motion.x,e.motion.y)){
            m_mouseState =1;
            ActivityEvent(100 + m_sNum);
            return true;
        }
        else m_mouseState =0;
        return false;
    }
    return false;
}

void GalSelButton::SetY(int y)
{
    m_dst.y = y;
    int w,h;
    m_str.GetSize(w,h);
    m_str.SetPos(pRnd.GetW()/2-w/2,y+3);
}

bool GalSelButton::Killed()
{
    return m_killed;
}

