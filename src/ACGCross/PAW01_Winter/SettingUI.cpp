#include "ACGCross/PAW01_Winter/SettingUI.h"
#include "ACGCross/PAW01_Winter/Slider.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"
#include "Snow.h"
#include "ACGCross/PAW01_Winter/Game.h"
#include "ACGCross/PAW01_Winter/str2wstr.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalgameActivity.h"
#include <cstdlib>
#include <string>
#include <sstream>

using namespace ACGCross;
using namespace ::Snow;
using namespace std;

struct screen{
    wstring w;
    bool full;
};

SettingUI::SettingUI()
{
    SetLogicScreenSize(1024,r.Int("TITLE_LOGICSCREEN_HEIGHT"));

    m_font.Open(ACGCross::r.Str("AFX_FONT"),32);

    m_bgmSetting.SetPos(660,150+r.Int("CFG_YOFFSET"));
    m_bgmSetting.SetLength(300);


    m_bgmSetting.SetBKTex("GalGameSystem/sldbk.png");
    m_bgmSetting.SetHKTex("GalGameSystem/sld1.png");
    m_bgmSetting.SetHKTex_Mot("GalGameSystem/sld2.png");
    m_bgmSetting.SetHKTex_Down("GalGameSystem/sld3.png");

    m_bgmLab.Load(m_font,StringToWString(r.Str("CFG_BGMSTR")));
    m_bgmLab.SetPos(660,100+r.Int("CFG_YOFFSET"));
    //m_bgmLab.SetZoom(0.4);

    m_cvSetting.SetPos(660,250+r.Int("CFG_YOFFSET"));
    m_cvSetting.SetLength(300);

    m_cvSetting.SetBKTex("GalGameSystem/sldbk.png");
    m_cvSetting.SetHKTex("GalGameSystem/sld1.png");
    m_cvSetting.SetHKTex_Mot("GalGameSystem/sld2.png");
    m_cvSetting.SetHKTex_Down("GalGameSystem/sld3.png");

    m_cvLab.Load(m_font,StringToWString(r.Str("CFG_CVSTR")));
    m_cvLab.SetPos(660,200+r.Int("CFG_YOFFSET"));
   // m_cvLab.SetZoom(0.4);

    m_seSetting.SetPos(660,350+r.Int("CFG_YOFFSET"));
    m_seSetting.SetLength(300);

    m_seSetting.SetBKTex("GalGameSystem/sldbk.png");
    m_seSetting.SetHKTex("GalGameSystem/sld1.png");
    m_seSetting.SetHKTex_Mot("GalGameSystem/sld2.png");
    m_seSetting.SetHKTex_Down("GalGameSystem/sld3.png");

    m_seLab.Load(m_font,StringToWString(r.Str("CFG_SESTR")));
    m_seLab.SetPos(660,300+r.Int("CFG_YOFFSET"));
    //m_seLab.SetZoom(0.4);

    RegControl(m_seSetting);
    RegControl(m_cvSetting);
    RegControl(m_bgmSetting);

    RegControl(m_return);
    RegControl(m_title);

    screen screenmode[] ={
        {L"4:3-Win-1024x768",false},
        {L"4:3-Win-1280x960",false},
        {L"4:3-Full-1024x768",true},
        {L"4:3-Full-1280x960",true},
        {L"4:3-Full-Auto",true},
        {L"16:9-Win-1024x576",false},
        {L"16:9-Win-1280x720",false},
        {L"16:9-Full-1024x576",true},
        {L"16:9-Full-1280x720",true},
        {L"16:9-Full-Auto",true}
    };

    m_grp_title.Load(m_font,StringToWString(r.Str("CFG_SCREENMODE")));
    m_grp_title.SetPos(150,100+r.Int("CFG_YOFFSET"));
    //m_grp_title.SetZoom(0.4);
    for(int i = 0;i < 10;++i){
        wstring wo=screenmode[i].w;

        m_grp_list[i].Load(m_font,wo);
        m_grp_list[i].SetPos(150,150+30*i+r.Int("CFG_YOFFSET"));
        m_grp_list[i].SetZoom(0.9);

    }
    updateGrpState();

    m_return.SetNormalPic("GalGameSystem/config_ret1.png");
    m_title.SetNormalPic("GalGameSystem/config_tle1.png");
    m_return.SetMotionPic("GalGameSystem/config_ret2.png");
    m_title.SetMotionPic("GalGameSystem/config_tle2.png");
    m_return.SetDownPic("GalGameSystem/config_ret3.png");
    m_title.SetDownPic("GalGameSystem/config_tle3.png");

    m_return.SetPos(655,458+r.Int("CFG_YOFFSET"));
    m_title.SetPos(940,458+r.Int("CFG_YOFFSET"));
}

void SettingUI::updateGrpState(){
    for(int i = 0;i<10;++i){
        if(i != gameData.GetGrpMode())
            m_grp_list[i].SetAlpha(128);
    }
}

SettingUI::~SettingUI()
{

}

void SettingUI::OnShow()
{
    m_grpWarn = false;
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

    m_grpWarn_t.SetRenderTextColor(0xfa,0xd8,0xd1,255);
    m_grpWarn_t.Load(m_font,StringToWString(ACGCross::r.Str("CFG_WARN")));
    m_grpWarn_t.SetZoom(0.7);
    m_grpWarn_t.SetPos(638,528+ACGCross::r.Int("CFG_YOFFSET"));

    m_stat = SHOWING;
    m_timer.Reset();
    delete pixels;
}

void SettingUI::OnHide(){
    gameData.AddUpdateTask(-3);
    gameData.UpdateData();

    m_bgt.Clear();
    m_bgt_o.Clear();
    m_bg.Clear();
}

void SettingUI::OnDraw()
{
    pRnd.Clear();

    m_bgt_o.OnDraw();
    m_bgt.OnDraw();
    if(m_grpWarn) m_grpWarn_t.OnDraw();

    m_cvSetting.OnDraw();
    m_seSetting.OnDraw();
    m_bgmSetting.OnDraw();
    m_cvLab.OnDraw();
    m_bgmLab.OnDraw();
    m_seLab.OnDraw();

    m_grp_title.OnDraw();
    for(int i = 0; i<10;++i) m_grp_list[i].OnDraw();

    m_return.OnDraw();
    m_title.OnDraw();

    pMouse -> OnDraw();
}

void SettingUI::OnNext()
{
    SDL_SetRenderDrawColor(pRnd,0x02,0x87,0xD4,255);
    if(m_stat == SHOWING){
        float per = ArcFunc(float(m_timer.GetTimer()) / 500);
        if(per == -1){
            m_stat = NOR;
            m_return.SetAlpha(192);
            m_title.SetAlpha(192);
            m_return.SetPos(655,458+r.Int("CFG_YOFFSET"));
            m_title.SetPos(840,458+r.Int("CFG_YOFFSET"));

            m_cvSetting.SetValue(float(gameData.GetCVVol())/128);
            m_bgmSetting.SetValue(float(gameData.GetBGMVol())/128);
            m_seSetting.SetValue(float(gameData.GetSEVol())/128);
            m_cvSetting.SetAlpha(192);
            m_bgmSetting.SetAlpha(192);
            m_seSetting.SetAlpha(192);
            m_bgt.SetAlpha(128);
            m_bgt_o.SetAlpha(0);
            m_bgmLab.SetAlpha(255);
            m_cvLab.SetAlpha(255);
            m_seLab.SetAlpha(255);
            m_grp_title.SetAlpha(255);
            for(int i = 0;i<10;++i){
                m_grp_list[i].SetPos(int(50),150+30*i+r.Int("CFG_YOFFSET"));
                if(i == gameData.GetGrpMode()) m_grp_list[i].SetAlpha(255);
                else m_grp_list[i].SetAlpha(128);
            }
        }else{
            m_return.SetAlpha(192*per);
            m_title.SetAlpha(192*per);
            m_return.SetPos(655,458+r.Int("CFG_YOFFSET")+50*(1-per));
            m_title.SetPos(840,458+r.Int("CFG_YOFFSET")+50*(1-per));

            m_bgmLab.SetAlpha(255*per);
            m_cvLab.SetAlpha(255*per);
            m_seLab.SetAlpha(255*per);
            m_grp_title.SetAlpha(255*per);
            m_cvSetting.SetValue(per* float(gameData.GetCVVol())/128);
            m_bgmSetting.SetValue(per* float(gameData.GetBGMVol())/128);
            m_seSetting.SetValue(per* float(gameData.GetSEVol())/128);
            m_cvSetting.SetAlpha(192*per);
            m_bgmSetting.SetAlpha(192*per);
            m_seSetting.SetAlpha(192*per);
            m_bgt_o.SetAlpha(255 - 255*per);
            m_bgt.SetAlpha(128*per);
            for(int i = 0;i<10;++i){
                m_grp_list[i].SetPos(int(50+120*(1-per)),150+30*i+r.Int("CFG_YOFFSET"));
                if(i == gameData.GetGrpMode()) m_grp_list[i].SetAlpha(255*per);
                else m_grp_list[i].SetAlpha(128*per);
            }
        }
    }else if(m_stat == HIDING){
        float per = 1 - ArcFunc(float(m_timer.GetTimer()) / 500);
        if(per == 2){
            Return();
            gameData.UpdateData();
            SDL_SetRenderDrawColor(pRnd,0,0,0,0);
        }else{
            m_return.SetAlpha(192*per);
            m_title.SetAlpha(192*per);
            m_return.SetPos(655,458+r.Int("CFG_YOFFSET")+50*(1-per));
            m_title.SetPos(840,458+r.Int("CFG_YOFFSET")+50*(1-per));

            m_bgmLab.SetAlpha(255*per);
            m_cvLab.SetAlpha(255*per);
            m_seLab.SetAlpha(255*per);
            m_grp_title.SetAlpha(255*per);
            m_cvSetting.SetValue(per* float(gameData.GetCVVol())/128);
            m_bgmSetting.SetValue(per* float(gameData.GetBGMVol())/128);
            m_seSetting.SetValue(per* float(gameData.GetSEVol())/128);
            m_cvSetting.SetAlpha(192*per);
            m_bgmSetting.SetAlpha(192*per);
            m_seSetting.SetAlpha(192*per);
            m_bgt_o.SetAlpha(255*(1-per));
            m_bgt.SetAlpha(128*per);

            m_grpWarn_t.SetAlpha(255*per);
            for(Uint8 i = 0;i<10;++i){
                m_grp_list[i].SetPos(int(50+120*(1-per)),150+30*i+r.Int("CFG_YOFFSET"));
                if(i == gameData.GetGrpMode()) m_grp_list[i].SetAlpha(255*per);
                else m_grp_list[i].SetAlpha(128*per);
            }
        }
    }else if(m_stat == TITLING){
        float per = 1 - ArcFunc(float(m_timer.GetTimer()) / 500);
        if(per == 2){
            SDL_SetRenderDrawColor(pRnd,0xFF,0xFF,0xFF,255);
            gameData.UpdateData();
            Goto(pTitle);
        }else{
            SDL_SetRenderDrawColor(pRnd,0x3A + 0xC5*(1-per),0xE6 + 0x19*(1-per),0xFF,255);
            m_return.SetAlpha(192*per);
            m_title.SetAlpha(192*per);
            m_return.SetPos(55,458+r.Int("CFG_YOFFSET")+50*(1-per));
            m_title.SetPos(240,458+r.Int("CFG_YOFFSET")+50*(1-per));

            m_bgmLab.SetAlpha(255*per);
            m_cvLab.SetAlpha(255*per);
            m_seLab.SetAlpha(255*per);
            m_grp_title.SetAlpha(255*per);
            m_cvSetting.SetValue(per* float(gameData.GetCVVol())/128);
            m_bgmSetting.SetValue(per* float(gameData.GetBGMVol())/128);
            m_seSetting.SetValue(per* float(gameData.GetSEVol())/128);
            m_cvSetting.SetAlpha(192*per);
            m_bgmSetting.SetAlpha(192*per);
            m_seSetting.SetAlpha(192*per);
            //m_bgt_o.SetAlpha(255*per);
            m_bgt.SetAlpha(128*per);

            m_grpWarn_t.SetAlpha(255*per);
            for(int i = 0;i<10;++i){
                m_grp_list[i].SetPos(int(650+120*(1-per)),150+30*i+r.Int("CFG_YOFFSET"));
                if(i == gameData.GetGrpMode()) m_grp_list[i].SetAlpha(255*per);
                else m_grp_list[i].SetAlpha(128*per);
            }
        }
    }
}

void SettingUI::OnEvent(Control* c, const Sint32 msg)
{
    if(m_stat != NOR) return;
    else if(c == &m_return && msg == 3){
        m_stat = HIDING;
        m_timer.Reset();
    }else if(c == &m_title && msg == 3){
        m_stat = TITLING;
        m_timer.Reset();
    }
    else if(msg == 1){  //如果是滑块变动了
        Uint8 vol = 128 * ((Slider*)c) -> GetValue();
        if(c == &m_bgmSetting){
            gameData.SetBGMVol(vol);
        }else if(c == &m_seSetting){
            gameData.SetSEVol(vol);
        }else if(c == &m_cvSetting){
            gameData.SetCVVol(vol);
        }
        ((Galgame::GalgameActivity*)pGal) ->SetVol(gameData.GetBGMVol(),gameData.GetSEVol(),gameData.GetCVVol());
    }
}

void SettingUI::OnEvent(const SDL_Event& e){

    pMouse -> OnEvent(e);
    //if(e.type == SDL_MOUSEBUTTONDOWN) PNT("MOUSE:"<<e.button.x<<","<<e.button.y);

    if(e.type == SDL_MOUSEMOTION){
        bool grpSel = false;
        for(int i = 0;i < 10;++i){
            if(m_grp_list[i].InRect(e.motion.x,e.motion.y)){
                grpSel = true;
                updateGrpState();
                m_grp_list[i].SetAlpha(255);
            }
        }
        if(!grpSel) updateGrpState();
    }
    else if(e.type == SDL_MOUSEBUTTONUP){
        SDL_Rect r = {51,ACGCross::r.Int("CFG_GRPHIG"),259,355};
        if(InRect(r,e.motion.x,e.motion.y)) for(int i = 0;i < 10;++i){
            if(m_grp_list[i].InRect(e.motion.x,e.motion.y)){
                gameData.SetGrpMode(i);
                gameData.AddUpdateTask(-3);
                m_grpWarn = true;
                //PNT("GRP:"<<i);
                break;
            }
        }
        updateGrpState();
    }

}
