#include "ACGCross/PAW01_Winter/SaveUI.h"
#include "ACGCross/PAW01_Winter/Game.h"
#include "ACGCross/PAW01_Winter/Title/Title.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalgameActivity.h"

using namespace ::Snow;
using namespace ACGCross;
using namespace ACGCross::Galgame;
using namespace std;
SaveUI::SaveUI()
{
    SetLogicScreenSize(1024,r.Int("TITLE_LOGICSCREEN_HEIGHT"));
    //ctor
}

void SaveUI::DLoad(int s)
{
    m_dLoad = s;
    m_dLoad_fg = 0;
}


void SaveUI::OnShow()
{
    m_dLoad = -1;
    if(!m_callByTitle){
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
        FastBlurARGB8888(m_bg,3);
        m_bgt.Load(m_bg);
        m_bgt.SetPos(0,0);
        m_bgt.SetZoom(pRnd.GetW(),pRnd.GetH());

        delete pixels;
    }

    //SetSaveButtons
    int y = r.Int("SAV_BESTLEFT_Y");
    int num = 1;

    //DBG
    //auto pSur = SDL_LoadBMP("0.bmp");
    Surface noData;
    noData.Load("GalGameSystem/saveUI_NODATA.png");

    for(int i = 0;i < 4;++i){
        int x = r.Int("SAV_BESTLEFT_X");
        for(int j = 0;j < 4;++j){
            if(num >= 16) num = -1;
            //PNT("SAVE UI SETTING BUTTON:"<<num<<":"<<x<<","<<y);
            m_saves[i][j] = new SaveButton(num);
            if(num != -1){
                if(gameData.GetDataExist(num)){
                    Uint32 rm,gm,bm,am;int bpp;
                    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_RGB24,
                                               &bpp,&rm,&gm,&bm,&am);
                    auto pSur = SDL_CreateRGBSurface(0,160,90,24,rm,gm,bm,am);

                    Uint8* pScreenDst = (Uint8*)((void*)gameData[num]);
                    pScreenDst += 65536 - 160*90*3;
                    SDL_LockSurface(pSur);
                    memcpy(pSur->pixels,pScreenDst,160*90*3);
                    SDL_UnlockSurface(pSur);
                    m_saves[i][j] ->LoadSurface(pSur);
                    SDL_FreeSurface(pSur);
                }
                else
                    m_saves[i][j] ->LoadSurface(noData);
            }else{
                ::Snow::Surface sur;
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
    if(m_callByTitle) pTitle -> OnDraw();

    else{
        pRnd.Clear();
        m_bgt_o.OnDraw();
        m_bgt.OnDraw();
    }

    for(int i = 0;i <4;++i)
    for(int j = 0;j < 4;++j){
        m_saves[i][j] -> OnDraw();
    }

    if(m_stat == HIDING && m_dLoad != -1){
        SDL_SetRenderDrawBlendMode(pRnd,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(pRnd,0,0,0,m_dLoad_fg);
        //PNT("DLOADFG:"<<(int)m_dLoad_fg);
        pRnd.Clear();
    }
    pMouse -> OnDraw();
}

void SaveUI::OnNext()
{

    if(m_callByTitle) pTitle -> OnNext();
    else SDL_SetRenderDrawColor(pRnd,0x3A,0xE6,0xFF,255);
    if(m_stat == SHOWING){
        float per = ArcFunc(float(m_timer.GetTimer()) / 200);
        if(per == -1){
            m_stat = NOR;
            m_bgt.SetAlpha(192);
            m_bgt_o.SetAlpha(0);
            for(int i = 0;i <4;++i)
            for(int j = 0;j < 4;++j)
                m_saves[i][j] -> SetShowing(1);
        }else{
            m_bgt_o.SetAlpha(255 - 255*per);
            m_bgt.SetAlpha(192*per);
            for(int i = 0;i <4;++i)
            for(int j = 0;j < 4;++j)
                m_saves[i][j] -> SetShowing(per);
        }
    }else if(m_stat == HIDING){
        float per = 1 - ArcFunc(float(m_timer.GetTimer()) / 200);
        if(per == 2){
            if(m_dLoad!=-1){
                m_dLoad_fg = 255;
                ((GalgameActivity*)pGal) -> LoadSave(m_dLoad);
                Goto(pGal);
            }else Return();
        }else{
            m_dLoad_fg = 255 * (1-per);
            m_bgt_o.SetAlpha(255*(1-per));
            m_bgt.SetAlpha(192*per);
            for(int i = 0;i <4;++i)
            for(int j = 0;j < 4;++j)
                m_saves[i][j] -> SetShowing(per);
        }
    }
}

void SaveUI::OnEvent(const SDL_Event& e){
    pMouse -> OnEvent(e);
    if(e.type == SDL_MOUSEMOTION)
        MoveDuang(e.motion.x,e.motion.y);
}

void SaveUI::OnEvent(::Snow::Control* c, const Sint32 sav)
{
    if(sav == -1){
        NeedReturn();
        return;
    }else{
        if(gameData.GetDataExist(sav)){
            SDL_Point poi = ((SaveButton*)c) -> GetPos();
            m_actReally.SetInit(poi.x,poi.y,sav,(SaveButton*)c);
            Call(&m_actReally);
        }else{
            if(m_callByTitle) return;
            else{
                ((GalgameActivity*)pGal) -> SaveGame(sav);
                gameData.SetDataExist(sav,true);
                gameData.AddUpdateTask(-2);
                gameData.AddUpdateTask(sav);
                NeedReturn();
            }
        }
    }
}

void SaveUI::NeedReturn(){
        if(m_callByTitle && m_dLoad == -1) ((Galgame::Title*)pTitle) -> ShowButton();
        m_stat = HIDING;
        m_timer.Reset();
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
    m_button.SetAlpha(96);
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

SDL_Point SaveUI::SaveButton::GetPos()
{
    SDL_Point poi;
    m_button.GetPos(poi.x,poi.y);
    return poi;
}


void SaveUI::Really::SetInit(int x, int y, int dataNum,SaveButton* b)
{
    m_callByTitle = ((SaveUI*)pSaveUI) -> GetCallByTitle();
    m_saveBtn = b;
    m_bg.x = x;
    m_bg.y = y + 90;
    m_bg.w = 160;
    m_save.SetPos(x,y+90);
    if(m_callByTitle){
        m_load.SetPos(x,y+90);
        m_del.SetPos(x,y+90+37);
    }
    else {
        m_load.SetPos(x,y+90+37);
        m_del.SetPos(x,y+90+37*2);
    }
    m_saveNum = dataNum;
    //PNT("SAVEUI::REALLY::SETINIT");
}

SaveUI::Really::Really()
{
    m_save.SetNormalPic(r.Str("SAV_BUTTON_SAVE"));
    m_save.GetNormal() -> SetAlpha(192);
    m_save.SetMotionPic(r.Str("SAV_BUTTON_SAVE"));
    m_save.GetMotion() -> SetColor(0,255,255);
    m_save.GetMotion() -> SetAlpha(192);
    m_save.SetDownPic(r.Str("SAV_BUTTON_SAVE"));
    m_save.GetDown() -> SetAlpha(128);

    m_load.SetNormalPic(r.Str("SAV_BUTTON_LOAD"));
    m_load.GetNormal() -> SetAlpha(192);
    m_load.SetMotionPic(r.Str("SAV_BUTTON_LOAD"));
    m_load.GetMotion() -> SetColor(0,255,255);
    m_load.GetMotion() -> SetAlpha(192);
    m_load.SetDownPic(r.Str("SAV_BUTTON_LOAD"));
    m_load.GetDown() -> SetAlpha(128);

    m_del.SetNormalPic(r.Str("SAV_BUTTON_DEL"));
    m_del.GetNormal() -> SetAlpha(192);
    m_del.SetMotionPic(r.Str("SAV_BUTTON_DEL"));
    m_del.GetMotion() -> SetColor(0,255,255);
    m_del.GetMotion() -> SetAlpha(192);
    m_del.SetDownPic(r.Str("SAV_BUTTON_DEL"));
    m_del.GetDown() -> SetAlpha(128);
}

void SaveUI::Really::OnHide()
{

}

void SaveUI::Really::OnDraw()
{
    pSaveUI -> OnDraw();
    SDL_SetRenderDrawColor(pRnd,255,255,255,192);
    SDL_RenderFillRect(pRnd,&m_bg);
    m_del.OnDraw();
    m_load.OnDraw();
    if(!m_callByTitle) m_save.OnDraw();
    pMouse->OnDraw();
}

void SaveUI::Really::OnNext()
{
    pSaveUI -> OnNext();

    float per;
    switch(m_state){
    case SO_MENU:
        per = float(m_timer.GetTimer()) / 200;
        if(per >= 1){
            m_state = SO_BTN;
            m_timer.Reset();
            if(!m_callByTitle) m_bg.h = 3*37;
            else m_bg.h = 2*37;
        }
        else {
            per = ArcFunc(per);
            if(!m_callByTitle) m_bg.h = 3*37*per;
            else m_bg.h = 2*37*per;
        }
        break;
    case SO_BTN:
        per = float(m_timer.GetTimer()) / 200;
        if(per >= 1){
            m_state = NOR;
            if(!m_callByTitle) RegControl(m_save);
            RegControl(m_load);
            RegControl(m_del);
            m_save.GetNormal() -> SetAlpha(192);
            m_load.GetNormal() -> SetAlpha(192);
            m_del.GetNormal() -> SetAlpha(192);
        }
        else {
            per = ArcFunc(per);
            m_save.GetNormal() -> SetAlpha(192*per);
            m_load.GetNormal() -> SetAlpha(192*per);
            m_del.GetNormal() -> SetAlpha(192*per);
        }
        break;

        case HI_BTN:
        per = float(m_timer.GetTimer()) / 200;
        if(per >= 1){
            m_state = HI_MENU;
            m_save.GetNormal() -> SetAlpha(0);
            m_load.GetNormal() -> SetAlpha(0);
            m_del.GetNormal() -> SetAlpha(0);
            m_save.GetMotion() -> SetAlpha(0);
            m_load.GetMotion() -> SetAlpha(0);
            m_del.GetMotion() -> SetAlpha(0);
            m_timer.Reset();
        }
        else {
            per = 1 - ArcFunc(per);
            m_save.GetNormal() -> SetAlpha(192*per);
            m_load.GetNormal() -> SetAlpha(192*per);
            m_del.GetNormal() -> SetAlpha(192*per);
            m_save.GetMotion() -> SetAlpha(192*per);
            m_load.GetMotion() -> SetAlpha(192*per);
            m_del.GetMotion() -> SetAlpha(192*per);
        }
        break;

    case HI_MENU:
        per = float(m_timer.GetTimer()) / 200;
        if(per >= 1){
            m_save.GetMotion() -> SetAlpha(192);
            m_load.GetMotion() -> SetAlpha(192);
            m_del.GetMotion() -> SetAlpha(192);
            if(m_needRet) ((SaveUI*)pSaveUI) -> NeedReturn();
            Return();
        }
        else {
            per = ArcFunc(1 - per);
            if(!m_callByTitle) m_bg.h = 3*37*per;
            else m_bg.h = 2*37*per;
        }
        break;

        default:
            break;

    }
}


void SaveUI::Really::OnShow()
{

    m_needRet = false;
    m_state = SO_MENU;
    m_timer.Reset();

    m_save.GetNormal() -> SetAlpha(0);
    m_load.GetNormal() -> SetAlpha(0);
    m_del.GetNormal() -> SetAlpha(0);

    SDL_Event forKillState;
    forKillState.type = SDL_MOUSEMOTION;
    forKillState.motion.x = forKillState.motion.y = 0;
    m_save.OnEvent(forKillState,*this);
    m_load.OnEvent(forKillState,*this);
    m_del.OnEvent(forKillState,*this);
}

void SaveUI::Really::OnEvent(const SDL_Event& e)
{
    pMouse -> OnEvent(e);
    if(e.type == SDL_MOUSEBUTTONUP && m_state == NOR){
        UnRegAllControl();
        m_state = HI_BTN;
        m_timer.Reset();
    }else if(e.type == SDL_MOUSEMOTION){
        ((SaveUI*)pSaveUI) -> MoveDuang(e.motion.x,e.motion.y);
        SDL_Point poi = m_saveBtn -> GetPos();
        m_bg.x = poi.x;
        m_bg.y = poi.y + 90;
        m_save.SetPos(poi.x,poi.y+90);
        if(m_callByTitle){
            m_load.SetPos(poi.x,poi.y+90);
            m_del.SetPos(poi.x,poi.y+90+37);
        }
        else {
            m_load.SetPos(poi.x,poi.y+90+37);
            m_del.SetPos(poi.x,poi.y+90+37*2);
        }
    }
}

void SaveUI::Really::OnEvent(::Snow::Control* c,const Sint32 msg){
    if(msg == 4){
        UnRegAllControl();
        if(c == (Control*)&m_save){
            ((GalgameActivity*)pGal) -> SaveGame(m_saveNum);
            gameData.SetDataExist(m_saveNum,true);
            gameData.AddUpdateTask(-2);
            gameData.AddUpdateTask(m_saveNum);
        }else if(c == (Control*)&m_load){
            ((SaveUI*)pSaveUI) -> DLoad(m_saveNum);
        }else{
            gameData.SetDataExist(m_saveNum,false);
            gameData.AddUpdateTask(-2);
        }
        m_state = HI_BTN;
        m_timer.Reset();
        m_needRet = true;
    }
}
