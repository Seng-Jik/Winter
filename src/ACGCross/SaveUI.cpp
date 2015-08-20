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
    if(GetParent() == pGal){
        SDL_Rect r = {0,0,pRnd.GetPhW(),pRnd.GetPhH()};
        Uint32* pixels = (new Uint32 [pRnd.GetPhW()*pRnd.GetPhH()]);
        SDL_RenderReadPixels(pRnd,
                         &r,
                         SDL_PIXELFORMAT_ARGB8888,
                         (void*)pixels,
                         pRnd.GetPhW()*4);
        m_bg.Load(SDL_CreateRGBSurfaceFrom((void*)pixels,pRnd.GetPhW(),pRnd.GetPhH(),32,pRnd.GetPhW()*4,
                                           0x00FF0000,0X0000FF00,0x000000FF,0XFF000000));
        m_bgt_o.Load(m_bg);
        m_bgt_o.SetPos(0,0);
        m_bgt_o.SetZoom(pRnd.GetW(),pRnd.GetH());
        FastBlurARGB8888(m_bg,5);
        m_bgt.Load(m_bg);
        m_bgt.SetPos(0,0);
        m_bgt.SetZoom(pRnd.GetW(),pRnd.GetH());

        m_stat = SHOWING;
        m_timer.Reset();
        delete pixels;
    }
}

void SaveUI::OnHide()
{
    m_bgt.Clear();
    m_bgt_o.Clear();
    m_bg.Clear();
}

void SaveUI::OnDraw()
{
    if(GetParent() != pGal) GetParent() -> OnDraw();

    if(GetParent() == pGal){
        pRnd.Clear();
        m_bgt_o.OnDraw();
        m_bgt.OnDraw();
    }
}

void SaveUI::OnNext()
{
    if(GetParent() != pGal) GetParent() -> OnNext();
    if(GetParent() == pGal) SDL_SetRenderDrawColor(pRnd,0x3A,0xE6,0xFF,255);
    if(m_stat == SHOWING){
        float per = ArcFunc(float(m_timer.GetTimer()) / 500);
        if(per == -1){
            m_stat = NOR;
            m_bgt.SetAlpha(128);
            m_bgt_o.SetAlpha(0);
        }else{
            m_bgt_o.SetAlpha(255 - 255*per);
            m_bgt.SetAlpha(128*per);
        }
    }else if(m_stat == HIDING){
        float per = 1 - ArcFunc(float(m_timer.GetTimer()) / 500);
        if(per == 2){
            m_stat = NOR;
            Return();
        }else{
            m_bgt_o.SetAlpha(255*(1-per));
            m_bgt.SetAlpha(128*per);
        }
    }
}

void SaveUI::OnEvent(const SDL_Event& e){
    if(e.type == SDL_KEYUP) {
        if(GetParent() ==pTitle) ((Galgame::Title*)GetParent()) -> ShowButton();
        m_stat = HIDING;
        m_timer.Reset();
    }
}

void SaveUI::OnEvent(Core::Control*, const Uint32)
{
}
