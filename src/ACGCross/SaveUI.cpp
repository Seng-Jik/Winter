#include "SaveUI.h"
#include "Game.h"
#include "ACGCross/Title/Title.h"
#include "ACGCross/MathFunc.h"
using namespace Core;
using namespace ACGCross;
using namespace std;
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

        delete pixels;
    }

    //SetSaveButtons
    int y = r.Int("SAV_BESTLEFT_Y");
    int num = 1;

    //DBG
    auto pSur = SDL_LoadBMP("0.bmp");
    Surface noData;
    noData.Load("GalGameSystem/saveUI_NODATA.png");

    for(int i = 0;i < 4;++i){
        int x = r.Int("SAV_BESTLEFT_X");
        for(int j = 0;j < 4;++j){
            if(num >= 16) num = -1;
            PNT("SAVE UI SETTING BUTTON:"<<num<<":"<<x<<","<<y);
            m_saves[i][j] = new SaveButton(num);
            if(num != -1){
                if(gameData.GetDataExist(num)){
                    //TODO:加载存档预览图
                    m_saves[i][j] ->LoadSurface(pSur);
                }
                else
                    m_saves[i][j] ->LoadSurface(noData);
            }else{
                Core::Surface sur;
                sur.Load("GalGameSystem/saveUI_back.png");
                m_saves[i][j] -> LoadSurface(sur);
            }
            m_saves[i][j] ->SetPos(x,y);
            x -= 180;
            m_saves[i][j] ->SetShowing(0);
            num++;
            RegControl(*m_saves[i][j]);
        }
        y += 110;
    }

    int mouse_x,mouse_y;
    SDL_GetMouseState(&mouse_x,&mouse_y);
    MoveDuang(mouse_x,mouse_y);

    m_stat = SHOWING;
    m_timer.Reset();
}

void SaveUI::MoveDuang(int mouse_x,int mouse_y){
    for(int i = 0;i <4;++i)
    for(int j = 0;j < 4;++j)
        m_saves[i][j] -> SetPosOffset(-(mouse_x-512)*0.03,-(mouse_y-384)*0.03);
}

void SaveUI::OnHide()
{
    UnRegAllControl();
    m_bgt.Clear();
    m_bgt_o.Clear();
    m_bg.Clear();

    for(int i = 0;i <4;++i)
    for(int j = 0;j < 4;++j)
        delete m_saves[i][j];
}

void SaveUI::OnDraw()
{
    if(GetParent() != pGal) GetParent() -> OnDraw();

    if(GetParent() == pGal){
        pRnd.Clear();
        m_bgt_o.OnDraw();
        m_bgt.OnDraw();
    }

    for(int i = 0;i <4;++i)
    for(int j = 0;j < 4;++j){
        m_saves[i][j] -> OnDraw();
    }
}

void SaveUI::OnNext()
{

    if(GetParent() != pGal) GetParent() -> OnNext();
    if(GetParent() == pGal) SDL_SetRenderDrawColor(pRnd,0x3A,0xE6,0xFF,255);
    if(m_stat == SHOWING){
        float per = ArcFunc(float(m_timer.GetTimer()) / 200);
        if(per == -1){
            m_stat = NOR;
            m_bgt.SetAlpha(128);
            m_bgt_o.SetAlpha(0);
            for(int i = 0;i <4;++i)
            for(int j = 0;j < 4;++j)
                m_saves[i][j] -> SetShowing(1);
        }else{
            m_bgt_o.SetAlpha(255 - 255*per);
            m_bgt.SetAlpha(128*per);
            for(int i = 0;i <4;++i)
            for(int j = 0;j < 4;++j)
                m_saves[i][j] -> SetShowing(per);
        }
    }else if(m_stat == HIDING){
        float per = 1 - ArcFunc(float(m_timer.GetTimer()) / 200);
        if(per == 2){
            m_stat = NOR;
            Return();
        }else{
            m_bgt_o.SetAlpha(255*(1-per));
            m_bgt.SetAlpha(128*per);
            for(int i = 0;i <4;++i)
            for(int j = 0;j < 4;++j)
                m_saves[i][j] -> SetShowing(per);
        }
    }
}

void SaveUI::OnEvent(const SDL_Event& e){
    if(e.type == SDL_MOUSEMOTION)
        MoveDuang(e.motion.x,e.motion.y);
}

void SaveUI::OnEvent(Core::Control* c, const Sint32 sav)
{
    if(sav == -1){
        if(GetParent() ==pTitle) ((Galgame::Title*)GetParent()) -> ShowButton();
        m_stat = HIDING;
        m_timer.Reset();
    }
}

void SaveUI::SaveButton::OnDraw()
{
    m_button.OnDraw();
}

void SaveUI::SaveButton::LoadSurface(SDL_Surface* pSur)
{
    m_button.Load(pSur);
    m_button.SetBlend(SDL_BLENDMODE_BLEND);
    m_button.SetColor(0x3A,0xE6,0xFF);
    m_button.SetAlpha(192);
}

bool SaveUI::SaveButton::OnEvent(const SDL_Event& e, Activity& a)
{
    if(e.type == SDL_MOUSEMOTION){
        if(m_button.InRect(e.motion.x,e.motion.y)){
            m_button.SetColor(255,255,255);
        }else
            m_button.SetColor(0x3A,0xE6,0xFF);
    }else if(e.type == SDL_MOUSEBUTTONDOWN){
        if(m_button.InRect(e.button.x,e.button.y)){
            m_button.SetAlpha(128);
        }
    }else if(e.type == SDL_MOUSEBUTTONUP){
        m_button.SetAlpha(192);
        if(m_button.InRect(e.button.x,e.button.y)){
            ActivityEvent(m_saveID);
        }
    }
    return false;
}

void SaveUI::SaveButton::SetPos(int x, int y)
{
    m_button.SetPos(x,y);
    m_orgPos.x = x;
    m_orgPos.y = y;
}

void SaveUI::SaveButton::SetShowing(float per)
{
    m_button.SetAlpha(192*per);
    m_button.SetSrc(0,0,160,90*per);
    m_button.SetZoom(160,int(90*per));
}

void SaveUI::SaveButton::SetPosOffset(int x, int y)
{
    m_button.SetPos(x+m_orgPos.x,y+m_orgPos.y);
}
