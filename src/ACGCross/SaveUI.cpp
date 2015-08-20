#include "SaveUI.h"
#include "Game.h"
#include "ACGCross/Title/Title.h"
#include "ACGCross/MathFunc.h"
using namespace Core;
using namespace ACGCross;
SaveUI::SaveUI()
{
    //ctor
}

void SaveUI::OnShow()
{
    m_bgsur.Load(SDL_CreateRGBSurface(0,256,1,32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000));
    m_bgsur.Lock();
    for(int i = 0;i < 256;++i)
        m_bgsur.PSet(i,0, i*0x1000000u);

    m_bgsur.Unlock();
    m_bg.Load(m_bgsur);
    m_bg.SetZoom(400,pRnd.GetH());
    m_bg.SetAlpha(0);

    m_stat = OPENNING;

    m_timer.Reset();
}

void SaveUI::OnHide()
{

}

void SaveUI::OnNext()
{
    GetParent() -> OnNext();
    if(m_timer.GetTimer() > 500 && m_stat == OPENNING){
        float per = float(m_timer.GetTimer() - 500)/500;
        if(per >= 1.0) {
            m_stat = SELECTSAVE;
            m_bg.SetAlpha(255);
            m_bg.SetPos(0x40,-400,0);
        }
        else {
            m_bg.SetAlpha(255*ArcFunc(per));
            m_bg.SetPos(0x40,-200 - 200 * ArcFunc(per),0);
        }
    }else if(m_stat == CLOSING){
        float per = float(m_timer.GetTimer())/500;
        if(per >= 1.0) {
            m_bg.SetAlpha(0);
            Return();
        }
        else {
            m_bg.SetAlpha(255 - 255*ArcFunc(per));
            m_bg.SetPos(0x40,-400 + 200 * ArcFunc(per),0);
        }
    }
}

void SaveUI::OnDraw()
{
    GetParent() -> OnDraw();
    m_bg.OnDraw();
}

void SaveUI::OnEvent(const SDL_Event& e){
    if(e.type == SDL_KEYUP) {
        if(GetParent() ==pTitle) ((Galgame::Title*)GetParent()) -> ShowButton();
        m_stat = CLOSING;
        m_timer.Reset();
    }
}

void SaveUI::OnEvent(Core::Control*, const Uint32)
{
}
