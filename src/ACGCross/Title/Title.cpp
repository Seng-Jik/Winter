#include "ACGCross/Title/Title.h"
#include "ACGCross/Game.h"
#include "ACGCross/GalgameActivity/GalgameActivity.h"
#include "ACGCross/SaveUI.h"
#include <cstdlib>
#include <fstream>
using namespace std;
using namespace Core;
using namespace ACGCross::Galgame;

void Title::ShowButton(){
    m_stat = 2;
    m_fpsCounter.Reset();
    m_visableBtn[0] -> Show();
}

void Title::HideButton(){
    m_stat = 4;
    m_fpsCounter.Reset();
    FOR_EACH(p,m_visableBtn.begin(),m_visableBtn.end())
        (*p) -> Hide();
}

void Title::FadeToBlackAndOpr(){
    m_stat = 3;
    m_fpsCounter.Reset();
    if(!m_bSta.GetHidden()) FOR_EACH(p,m_visableBtn.begin(),m_visableBtn.end())
        (*p) -> Hide();
}

void Title::OnDraw()
{
    if(!m_lbg.Finished()){
        SDL_SetRenderDrawColor(pRnd,255,255,255,255);
        pRnd.Clear();

        m_acgcrossLogo.SetAlpha(m_lbg.GetPer()*255);
        m_fk.SetAlpha(m_lbg.GetPer()*255);
    }


    m_mbg.OnDraw();
    m_fk.OnDraw();
    m_snow.OnDraw();
    m_lbg.OnDraw();
    m_acgcrossLogo.OnDraw();

    FOR_EACH(p,m_visableBtn.begin(),m_visableBtn.end())
        (*p) -> OnDraw();

    if(m_stat == 3 || m_stat == 4){
        float per = float(m_fpsCounter.GetTimer())/40;

        SDL_SetRenderDrawColor(pRnd,0,0,0,per*255);
        if(m_stat == 3) pRnd.Clear();
        if(per>=1.0) {
            SDL_SetRenderDrawColor(pRnd,0,0,0,255);
            if(m_stat == 3) pRnd.Clear();
            m_dosomething = true;
            m_stat = 0;
        }
    }
}

void Title::OnNext()
{

    if(m_stat == 2) for(unsigned int i = 0;i < m_visableBtn.size()-1;i++){
        if(m_fpsCounter == Uint8(5+5*i))
            m_visableBtn[i+1] -> Show();
    }

    /*
    if(m_stat == 3||m_stat ==4) for(unsigned int i = 0;i < m_visableBtn.size()-1;i++){
        if(m_fpsCounter == Uint8(5+5*i))
            m_visableBtn[i+1] -> Hide();
    }*/

    if(m_stat == 2 && m_fpsCounter >= 5*m_visableBtn.size()) m_stat = 0;

    m_mbg.OnNext();
    m_snow.OnNext();
    m_lbg.OnNext();

    FOR_EACH(p,m_visableBtn.begin(),m_visableBtn.end())
        (*p) -> OnNext();

    if(m_stat == 4){
        //todo
    }

    if(m_dosomething) {
        gameData.UpdateData();
        m_dosomething = false;

        if(m_btnOpr == &m_bExit) exit(0);
        else if(m_btnOpr == &m_bSta) {
            ((GalgameActivity*)pGal) -> LoadSave(-1);
            Goto(pGal);
            JumpDraw();
        }else if(m_btnOpr == &m_bContinue) {
            ((GalgameActivity*)pGal) -> LoadSave(0);
            Goto(pGal);
            JumpDraw();
        }else if(m_btnOpr == &m_bLoad){
            ((SaveUI*)pSaveUI) -> SetCallByTitle(true);
            Call(pSaveUI);
        }
        m_btnOpr = nullptr;
    }

}

void Title::OnHide()
{
    /*m_snow.Stop();
    m_mbg.Quit();
    m_snow.Quit();
    m_lbg.Destory();
    m_acgcrossLogo.Clear();
    m_fk.Clear();

    m_visableBtn.clear();*/
    pTitle = new Title;
    delete this;

}

void Title::OnInit(){

}

void Title::OnShow()
{
    m_dataNum = -1;

    m_fpsCounter.Reset();
    m_mbg.SetDstRect(0,0);
    m_mbg.Init(r.Str("TITLE_MBG_IMAGE"));

    m_mbg.SetFadeTime(r.Int("TITLE_MBG_FADETIME"));
    m_mbg.FadeIn();


    m_snow.Init();
    m_snow.SetSpeed(r.Int("TITLE_SNOW_SPEED"),r.Int("TITLE_SNOW_SPEEDP"));
    m_snow.SetVolume(50);
    m_snow.SetWind(0,10);
    m_snow.Start();

    m_lbg.Init();
    m_lbg.Show();
    m_lbg.SetFadeSpeed(1000);
    m_lbg.SetTopAlpha(192);

    m_acgcrossLogo.Load(r.Str("TITLE_ACGCROSS_LOGO"));
    m_acgcrossLogo.SetPos(0x44,-160,-40);
    m_acgcrossLogo.SetAlpha(0);

    m_fk.Load("Title/fk.png");
    m_fk.SetPos(0,0);
    m_fk.SetZoom(r.Int("TITLE_FKZOOM_W"),r.Int("TITLE_FKZOOM_H"));
    m_fk.SetAlpha(0);

    if(gameData.GetCSaveExist()){
        m_bContinue.SetNormalPic(r.Str("TITLE_BUTTON_CONTINUE"));
        m_bContinue.SetMotionPic(r.Str("TITLE_BUTTON_CONTINUE_M"));
        m_bContinue.SetDownPic(r.Str("TITLE_BUTTON_CONTINUE_D"));
        //m_bLoad.SetPos(738,70);
        m_bContinue.Button::Hide();
        m_visableBtn.push_back(&m_bContinue);
    }

    m_bSta.SetNormalPic(r.Str("TITLE_BUTTON_START"));
    m_bSta.SetMotionPic(r.Str("TITLE_BUTTON_START_M"));
    m_bSta.SetDownPic(r.Str("TITLE_BUTTON_START_D"));
    //m_bSta.SetPos(813,70);
    m_bSta.Button::Hide();
    m_visableBtn.push_back(&m_bSta);

    if(gameData.GetSaveExist()){
        m_bLoad.SetNormalPic(r.Str("TITLE_BUTTON_LOAD"));
        m_bLoad.SetMotionPic(r.Str("TITLE_BUTTON_LOAD_M"));
        m_bLoad.SetDownPic(r.Str("TITLE_BUTTON_LOAD_D"));
        //m_bLoad.SetPos(738,70);
        m_bLoad.Button::Hide();
        m_visableBtn.push_back(&m_bLoad);
    }

    if(gameData.GetCGExist()){
        m_bCG.SetNormalPic(r.Str("TITLE_BUTTON_CG"));
        m_bCG.SetMotionPic(r.Str("TITLE_BUTTON_CG_M"));
        m_bCG.SetDownPic(r.Str("TITLE_BUTTON_CG_D"));
        //m_bCG.SetPos(663,70);
        m_bCG.Button::Hide();
        m_visableBtn.push_back(&m_bCG);
    }

    m_bExit.SetNormalPic(r.Str("TITLE_BUTTON_EXIT"));
    m_bExit.SetMotionPic(r.Str("TITLE_BUTTON_EXIT_M"));
    m_bExit.SetDownPic(r.Str("TITLE_BUTTON_EXIT_D"));
    //m_bExit.SetPos(588,70);
    m_bExit.Button::Hide();
    m_visableBtn.push_back(&m_bExit);

    int x = r.Int("TITLE_BUTTON_X_START");
    FOR_EACH(p,m_visableBtn.begin(),m_visableBtn.end()){
        (*p) -> SetPos(x,r.Int("TITLE_BUTTON_Y"));
        x -= 85;
    }

    m_fpsCounter.Reset();
    m_visableBtn[0] -> Show();
    m_stat = 2;

}

 Title::Title()
{
    SetLogicScreenSize(1024,r.Int("TITLE_LOGICSCREEN_HEIGHT"));
    RegControl(m_bSta);
    RegControl(m_bLoad);
    RegControl(m_bCG);
    RegControl(m_bExit);
    RegControl(m_bContinue);
}

void Title::OnEvent(const SDL_Event& e)
{
}

void Title::OnEvent(Control* c, const Sint32 msg)
{
    if(c == &m_bLoad && msg == 3){
        HideButton();
        //Call(pSaveUI);
        m_btnOpr = (TitleButton*)c;
        //m_dosomething = true;
        return;
    }
    m_btnOpr = (TitleButton*)c;
    if(c == &m_bExit || c == &m_bContinue || c == &m_bSta){
        if(msg == 3) FadeToBlackAndOpr();
    }else{
        if(msg == 3) HideButton();
    }
}

