#include "ACGCross/PAW01_Winter/FadeLable.h"

using namespace ACGCross;

FadeLable::FadeLable()
{
    m_stat = NOTHING;
}

void FadeLable::Show()
{
    m_stat = SHOWING;
    m_timer.Reset();
}

void FadeLable::Hide()
{
    m_stat = HIDING;
    m_timer.Reset();
}

void FadeLable::UpdateText()
{
    m_stat = HIDING_CHG;
    m_timer.Reset();
}

void FadeLable::UpdateTextFast(){
    m_stat = NOTHING;
    if(m_text.empty()) m_tex.Clear();
    m_tex.SetRenderTextColor(m_color[0],m_color[1],m_color[2]);
    m_tex.Load(*m_fnt,m_text,true);
    m_tex.SetAlpha(255);
    m_tex.SetPos(m_pos[0],m_pos[1]);

    {
        int w;
        int t_w,t_h;
        m_tex.GetSize(t_w,t_h);
        float w_zoom = float(m_size) / t_h;
        w = w_zoom * t_w;
        m_tex.SetZoom(w,m_size);
    }
}

void FadeLable::SetFont(Core::Font* f)
{
    m_fnt = f;
}

void FadeLable::SetPos(int x, int y)
{
    m_pos[0] = x;
    m_pos[1] = y;
}

void FadeLable::SetSize(int hei)
{
    m_size = hei;
}

void FadeLable::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
}

void FadeLable::SetAlpha(Uint8 alp)
{
    m_tex.SetAlpha(alp);
}

void FadeLable::SetText(const std::wstring& s)
{
    m_text = s;
}

void FadeLable::OnNext()
{
    if(m_stat == HIDING || m_stat == HIDING_CHG){
        float per = float(m_timer.GetTimer())/m_fpsTime;
        if(per >= 1.0){
            m_tex.SetAlpha(0);
            if(m_stat == HIDING_CHG){
                m_stat = SHOWING;
                m_timer.Reset();
                m_tex.SetRenderTextColor(m_color[0],m_color[1],m_color[2]);
                m_tex.Load(*m_fnt,m_text,true);
                m_tex.SetAlpha(0);
                m_tex.SetPos(m_pos[0],m_pos[1]);

                {
                    int w;
                    int t_w,t_h;
                    m_tex.GetSize(t_w,t_h);
                    float w_zoom = float(m_size) / t_h;
                    w = w_zoom * t_w;
                    m_tex.SetZoom(w,m_size);
                }

            }else m_stat = NOTHING;
        }else m_tex.SetAlpha(255*(1-per));
    }else if(m_stat == SHOWING){
        float per = float(m_timer.GetTimer())/m_fpsTime;
        if(per >=1.0){m_stat = NOTHING;m_tex.SetAlpha(255);}
        else m_tex.SetAlpha(255*per);
    }
}

void FadeLable::OnDraw()
{
    m_tex.OnDraw();
}

void FadeLable::SetFadeTime(int ms){
    m_fpsTime = Core::Time2Fps(ms);
}

std::wstring& FadeLable::GetText(){
    return m_text;
}
