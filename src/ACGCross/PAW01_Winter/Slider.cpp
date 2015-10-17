#include "ACGCross/PAW01_Winter/Slider.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"
#include "ACGCross/PAW01_Winter/Game.h"
using namespace ACGCross;
using namespace ::Snow;

bool Slider::OnEvent(const SDL_Event& e, Activity& a)
{
    pMouse -> OnEvent(e);
    SDL_Rect rHk = {m_r.x+m_hkxpos_xd,m_r.y,m_hkw,m_hkh};
    if(e.type == SDL_MOUSEMOTION){
        if(m_mouse == DOW){
            int len = e.motion.x - m_r.x-m_mouse_offset;
            if(len <= 0) SetValue(0);
            else if(len >= m_r.w) SetValue(1);
            else{
                m_hkxpos_xd = len;
                /*float alpLen = ArcFunc(GetValue());
                m_hkd.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
                m_lener.SetZoom(m_hkxpos_xd,m_r.h);
                if(alpLen < 0) m_lener.SetAlpha(GetValue()*255);
                else m_lener.SetAlpha(alpLen*255);*/
                SetValue(GetValue());
            }
            ActivityEvent(1);
            return true;
        }else if(InRect(rHk,e.motion.x,e.motion.y)){
            m_mouse = MOT;
            return true;
        }else{
            m_mouse = NOR;
            return false;
        }
    }else if(e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && InRect(rHk,e.button.x,e.button.y)){
        m_mouse = DOW;
        m_mouse_offset = e.button.x - m_hkxpos_xd - m_r.x;
        ActivityEvent(2);
        return true;
    }else if(e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT && m_mouse == DOW){
        m_mouse = NOR;
        SetValue(GetValue());
        ActivityEvent(1);
        ActivityEvent(3);
        return true;
    }else return false;
}

void Slider::SetValue(float f)
{
    m_hkxpos_xd = f * m_r.w;
    m_hk.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
    m_hkd.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
    m_hkm.SetPos(m_r.x + m_hkxpos_xd,m_r.y);

    m_hkd.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
    m_lener.SetZoom(m_hkxpos_xd,m_r.h);
    float arcAlp = ArcFunc(f);
    if(arcAlp < 0) m_lener.SetAlpha(f*255);
    else m_lener.SetAlpha(arcAlp*255);
}

void Slider::SetPos(int x, int y)
{
    m_r.x = x,m_r.y = y;
    m_re = m_r;
    m_re.w+= m_hkw;
    m_lener.SetPos(m_r.x,m_r.y);
    m_hk.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
    m_hkd.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
    m_hkm.SetPos(m_r.x + m_hkxpos_xd,m_r.y);
}

void Slider::SetLength(int w)
{
    m_r.w = w;
    m_re = m_r;
    m_re.w += m_hkw;

    int wk;
    m_lener.GetSize(wk,m_r.h);
    m_lener.SetZoom(m_hkxpos_xd,m_r.h);
}

void Slider::SetAlpha(Uint8 alp)
{
    m_hk.SetAlpha(alp);
    m_hkm.SetAlpha(alp);
    m_hkd.SetAlpha(alp);
}

void Slider::OnDraw()
{
    m_lener.OnDraw();
    switch(m_mouse){
    case MOT:
        m_hkm.OnDraw();break;
    case NOR:
        m_hk.OnDraw();break;
    case DOW:
        m_hkd.OnDraw();break;
    }
}

void Slider::SetBKTex(const std::string& s)
{
    m_lener.Load(s);
    m_re = m_r;
    m_re.w += m_hkw;

    int wk;
    m_lener.GetSize(wk,m_r.h);
    m_lener.SetZoom(m_hkxpos_xd,m_r.h);
}

void Slider::SetHKTex(const std::string& s)
{
    m_hk.Load(s);
    m_hk.GetSize(m_hkw,m_hkh);

    m_re = m_r;
    m_re.w += m_hkw;

    int wk;
    m_lener.GetSize(wk,m_r.h);
    m_lener.SetZoom(m_hkxpos_xd,m_r.h);
}

void Slider::SetHKTex_Mot(const std::string& s)
{
    m_hkm.Load(s);
}

void Slider::SetHKTex_Down(const std::string& s)
{
    m_hkd.Load(s);
}
