#include "ACGCross/PAW01_Winter/Ending.h"
#include "ACGCross/PAW01_Winter/Game.h"

using namespace ACGCross;
Ending::Ending(){
    SetLogicScreenSize(1024,r.Int("TITLE_LOGICSCREEN_HEIGHT"));
}
void Ending::OnShow()
{
    m_timer.Reset();
    //m_bgm.Load(r.Str("ED_BGM"));
    m_ed.Load(r.Str("ED_STAFF"));
    m_edtime = r.Int("ED_TIME") - 5000;

    int w,h;
    m_ed.GetSize(w,h);
    //m_ed.SetPos(1024 / 2 - w / 2,r.Int("TITLE_LOGICSCREEN_HEIGHT"));
    m_staffHeightCount = r.Int("ED_STAFF_STOP") + r.Int("TITLE_LOGICSCREEN_HEIGHT");
    m_edx = 1024 / 2 - w / 2;
    //m_bgm.Play();
}

void Ending::OnHide()
{
    m_ed.Clear();
    m_bgm.Stop(1000);
}

void Ending::OnDraw()
{
    SDL_SetRenderDrawColor(Core::pRnd,255,255,255,255);
    Core::pRnd.Clear();
    m_ed.OnDraw();
}

void Ending::OnNext()
{
    float per = float(m_timer.GetTimer()) / m_edtime;
    if(per >= 1) {
        if(m_timer.GetTimer() - m_edtime >= 5000){
            if(m_timer.GetTimer() - m_edtime < 6000){
                float fper = float(m_timer.GetTimer() - m_edtime - 5000) / 1000;
                m_ed.SetAlpha(255-fper*255);
            }
            else Goto(pTitle);
        }
    }
    else m_ed.SetPos(m_edx,int(r.Int("TITLE_LOGICSCREEN_HEIGHT") - per * m_staffHeightCount));
}
