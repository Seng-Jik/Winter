#include "ACGCross/PAW01_Winter/GalgameActivity/TextWindow.h"
#include "ACGCross/PAW01_Winter/game.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalgameActivity.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"
#define MOVE_EFFECT_H 90
using namespace ACGCross::Galgame;
using namespace ::Snow;

TextWindow::TextWindow()
{
    //ctor
    m_posy = r.Int("GAL_WINDOW_NORMAL_Y");
    m_fadeFpsTime = Snow::Time2Frame(400);
}

void TextWindow::Quit()
{
    m_tex.Clear();
}

void TextWindow::Init()
{
    m_tex.Load(r.Str("GAL_WINDOW_NORMAL_PIC"));
    m_tex.SetPos(0,m_posy);
    m_tex.SetZoom(1024,r.Int("GAL_WINDOW_NORMAL_ZOOMY"));
    m_stat = HIDDEN;
}

void TextWindow::OnNext()
{
    if(m_stat == HIDDEN || m_stat == NOTHING) return;
    else if(m_stat == SHOWING){
        float per = ArcFunc(float(m_fps.GetTimer())/m_fadeFpsTime);
        if(per == -1.0){
            m_stat = NOTHING;
            m_tex.SetPos(0,m_posy);
            m_tex.SetAlpha(255);
            FOR_EACH(p,m_widl.begin(),m_widl.end()){
                (*p) -> TWPos((*p) -> m_TWx,(*p) -> m_TWy);
                (*p) -> TWAlpha(255);
            }

            pTextBox -> Show();
            ((GalgameActivity*)(pGal)) -> m_name.Show();
            return;
        }
        m_tex.SetPos(int(0),int(m_posy+MOVE_EFFECT_H*(1-per)));
        m_tex.SetAlpha(255*per);
        FOR_EACH(p,m_widl.begin(),m_widl.end()){
                (*p) -> TWPos((*p) -> m_TWx,(*p) -> m_TWy + MOVE_EFFECT_H*(1-per));
                (*p) -> TWAlpha(255*per);
        }
    }else if(m_stat == HIDING_OT){
        if(pTextBox -> Finished()) {
            m_fps.Reset();
            m_stat = HIDING;
        }
    }else if(m_stat == HIDING){
        float per = ArcFunc(float(m_fps.GetTimer())/m_fadeFpsTime);
        if(per == -1.0){
            m_stat = NOTHING;
            m_tex.SetPos(0,m_posy);
            m_tex.SetAlpha(0);
            FOR_EACH(p,m_widl.begin(),m_widl.end()){
                (*p) -> TWAlpha(0);
            }
            return;
        }
        m_tex.SetPos(int(0),int(m_posy+MOVE_EFFECT_H*per));
        m_tex.SetAlpha(255-255*per);
        FOR_EACH(p,m_widl.begin(),m_widl.end()){
                (*p) -> TWPos((*p) -> m_TWx,(*p) -> m_TWy + MOVE_EFFECT_H*per);
                (*p) -> TWAlpha(255-255*per);
        }
    }
}

void TextWindow::OnDraw()
{
    if(m_stat != HIDDEN) m_tex.OnDraw();
}

void TextWindow::RegWig(Widget& w, int x, int y)
{
    w.m_TWx = x,w.m_TWy = y;
    m_widl.push_back(&w);
}

void TextWindow::Hide()
{
    m_fps.Reset();
    m_stat = HIDING_OT;
    pTextBox -> Hide();
    ((GalgameActivity*)pGal) -> m_name.Hide();
}

