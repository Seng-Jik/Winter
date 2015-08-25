#include "ACGCross\GalgameActivity\GalgameActivity.h"
#include "ACGCross/str2wstr.h"
#include "ACGCross/Game.h"
#include "ACGCross/SaveUI.h"

#define IFFAST if(fast)
#define REGSMCMD(s,u) m_SMEProc_map[s] = (SMEProcUnit*)&u

using namespace std;
using namespace Core;
using namespace ACGCross;
using namespace ACGCross::Galgame;

using namespace SMI;

void GalgameActivity::SetWindowStyle_Normal()
{
    //int x,y;
    //Pos(x,y,0x04,50,-160);
    m_text -> SetRect(50+128,r.Int("TITLE_LOGICSCREEN_HEIGHT") - 160,819-50-128,512);    //x offseted 128px

    m_text -> SetEffectSpeed(150);
    m_text -> SetColor(255,255,255);
    m_text -> SetFontSize(r.Int("GAL_WORDSIZE"));
    m_text -> SetSpeed(r.Int("GAL_SAYSPEED"));

    m_name.SetTextBox(m_text);
    m_name.SetFadeTime(70);
    m_name.SetColor(255,255,255);
    m_name.SetSize(48);
    m_name.SetPos(r.Int("GAL_NAMECARD_X"),r.Int("GAL_NAMECARD_Y"));
}

void GalgameActivity::SMEClick()
{
    SMEUpdateStat();
    if(m_SMEProc_stat == PROCESSING){
        SMEProc(true);
        while(m_SMEProc_sme.type != SAYEND){
            m_SMEProc_smi.PullEvent(m_SMEProc_sme);
            m_SMEProc_ProcCalled = false;
            m_SMEProc_FastProcCalled = false;
            SMEProc(true);
            SMEUpdateStat();
        }
        m_SMEProc_stat = WAITCLICK;
        SMEText_lb();
    }
    else if(m_SMEProc_stat == WAITCLICK){
        LowerSaveGame();
        if(!gameData.GetCSaveExist()){
            gameData.SetDataExist(0,true);
            gameData.AddUpdateTask(-2);
        }

        m_SMEProc_stat = PROCESSING;
        m_text_cmdtarget.ClearTextLog();
        m_text -> Clear();
        m_text -> SetColor(255,255,255);
        corner.Hide();
        m_name_isEmpty = true;
        m_cv.Stop(250);
        //todo:when text continue.
    }else PNT("SMEProcessor:UNKNOW state");
}

void GalgameActivity::SMENext()
{
    SMEUpdateStat();
    if(m_SMEProc_skipping){
        m_text -> StopSpeak();
        m_SMEProc_skipFPSClicks ++;
        if(m_SMEProc_skipFPSClicks >=5){
            SMEClick();
            m_SMEProc_skipFPSClicks = 0;
        }
    }else if(m_SMEProc_autoing){
        if(m_SMEProc_autoTimer.GetTimer()>=1000 &&
           !m_cv.Playing() && m_SMEProc_stat == WAITCLICK)
            SMEClick();
    }
    while(m_SMEProc_stat == FINISHED && m_textWindow.Finished()){
        m_SMEProc_smi.PullEvent(m_SMEProc_sme);
        m_SMEProc_ProcCalled = false;
        m_SMEProc_FastProcCalled = false;
        SMEProc();
        SMEUpdateStat();
    }
}

void GalgameActivity::SMEUpdateStat()
{
    if(m_SMEProc_sme.type == SMI::SMEType::END) m_SMEProc_stat = SMEProcessorState::END;
    else if(m_SMEProc_sme.type == SAY || (m_SMEProc_sme.type == SAYEND && m_SMEProc_stat != WAITCLICK))
    {
        if(m_SMEProc_Say -> SMEFinished(&m_SMEProc_sme)) {m_SMEProc_stat = FINISHED;}
        else m_SMEProc_stat = PROCESSING;
    }
    else switch(m_SMEProc_sme.type){
    case NAME:
        m_SMEProc_stat = FINISHED;break;
    case CMD:
        if(m_SMEProc_map.count(m_SMEProc_sme.cmd)){
            if(m_SMEProc_map[m_SMEProc_sme.cmd] -> SMEFinished(&m_SMEProc_sme))
                m_SMEProc_stat = FINISHED;
                else m_SMEProc_stat = PROCESSING;
        }

        break;
    default:
        break;
    }
}

void GalgameActivity::SMEText_lb(){
    corner.Show();
    textLog.push_back(m_text_cmdtarget.GetTextLog());
}

void GalgameActivity::SMEProc(bool fast)
{
    PNT("SMEProcessor:PROC BEGIN");
    if(!m_SMEProc_ProcCalled){
        if(m_SMEProc_sme.type ==SMEType::END) exit(0);
        switch(m_SMEProc_sme.type){
        case SAY:
            PNT("SMEProcessor:PROC say");
            if(m_name_isEmpty) {m_name.SetText(L"");m_name.UpdateText();}
            m_SMEProc_Say -> SMEProc(&m_SMEProc_sme);break;
        case SAYEND:
            PNT("SMEProcessor:PROC sayend");
            m_SMEProc_stat = WAITCLICK;SMEText_lb();
            m_SMEProc_autoTimer.Reset();
            break;
        case NAME:
            PNT("SMEProcessor:PROC name");
            m_name_isEmpty = false;
            m_SMEProc_Name -> SMEProc(&m_SMEProc_sme);
            break;
        case CMD:
            PNT("SMEProcessor:PROC cmd");
            if(m_SMEProc_map.count(m_SMEProc_sme.cmd)) m_SMEProc_map [m_SMEProc_sme.cmd] -> SMEProc(&m_SMEProc_sme);
            else PNT("SMEProcessor:UNKNOWN CMD "<<WStringToString(m_SMEProc_sme.cmd));
            break;
        default:
            PNT("SMEProcessor:PROC default");
            break;
        }
        m_SMEProc_ProcCalled = true;
    }
    PNT("SMEProcessor:PROC END");

    if(fast && !m_SMEProc_FastProcCalled){
        switch(m_SMEProc_sme.type){
        case SAY:
            PNT("SMEProcessor:FASTPROC say");
            m_SMEProc_Name -> SMEFastProc(&m_SMEProc_sme);
            m_SMEProc_Say -> SMEFastProc(&m_SMEProc_sme);
            break;
        case SAYEND:
            PNT("SMEProcessor:FASTPROC sayend");
            break;
        case NAME:
            PNT("SMEProcessor:FASTPROC name");
            m_SMEProc_Name -> SMEFastProc(&m_SMEProc_sme);break;
        case CMD:
            PNT("SMEProcessor:FASTPROC cmd");
            if(m_SMEProc_map.count(m_SMEProc_sme.cmd)) m_SMEProc_map [m_SMEProc_sme.cmd] -> SMEFastProc(&m_SMEProc_sme);
            //else PNT("UNREGED CMD:"<<WStringToString(m_SMEProc_sme.cmd));
            break;
        default:
            break;
        }
        m_SMEProc_FastProcCalled = true;
    }
    PNT("SMEProcessor:PROC END");
}

void GalgameActivity::HideWindow()
{
    if(m_SMEProc_stat == PROCESSING) SMEClick();
    if(m_clock -> GetVisible()) m_clock -> Hide();
    m_astat = WINDOWHIDDEN;
    m_text -> Hide();
    m_textWindow.Hide();
    m_name.Hide();
    corner.Hide();
}

void GalgameActivity::ShowWindow()
{
    m_astat = RUNNING;
    m_textWindow.Show();
    corner.Show();
}

GalgameActivity::GalgameActivity(TextBox* b,Clock* c):m_bgm_loader(&BGMLoader),m_mask(&m_bg)
{
    m_text = b;
    m_clock = c;
    PNT("GALGAMEACTIVITY CREATED!");
    SetLogicScreenSize(1024,r.Int("TITLE_LOGICSCREEN_HEIGHT"));
    RegControl(m_textWindow_X);
    RegControl(m_textWindow_save);
    RegControl(m_textWindow_skip);
    RegControl(m_textWindow_set);
    RegControl(m_textWindow_auto);

    //m_text -> Init();
    SetWindowStyle_Normal();


    m_SMEProc_smi.LoadStory(r.Str("SMI_STORYFILE"),false);
    m_SMEProc_stat = FINISHED;

    m_text_cmdtarget.SetText(m_text);

    m_textWindow.RegWig(m_textWindow_X,975,r.Int("GAL_TEXTWINDOW_XBUTTON_Y"));
    m_textWindow.RegWig(m_textWindow_save,r.Int("GAL_TEXTWINDOW_SAVE_X"),r.Int("GAL_TEXTWINDOW_SAVE_Y"));
    m_textWindow.RegWig(m_textWindow_skip,r.Int("GAL_TEXTWINDOW_SKIP_X"),r.Int("GAL_TEXTWINDOW_SKIP_Y"));
    m_textWindow.RegWig(m_textWindow_set,r.Int("GAL_TEXTWINDOW_SET_X"),r.Int("GAL_TEXTWINDOW_SET_Y"));
    m_textWindow.RegWig(m_textWindow_auto,r.Int("GAL_TEXTWINDOW_AUTO_X"),r.Int("GAL_TEXTWINDOW_AUTO_Y"));

    //reg SMEProcUnit
    m_SMEProc_Say = (SMEProcUnit*)&m_text_cmdtarget;
    m_SMEProc_Name = (SMEProcUnit*)&m_name;

    REGSMCMD(L"s",m_text_cmdtarget);    //T
    REGSMCMD(L"sd",m_text_cmdtarget);   //T
    REGSMCMD(L"w",*this);   //T
    REGSMCMD(L"fontsize",m_text_cmdtarget); //T
    REGSMCMD(L"deffontsize",m_text_cmdtarget);  //T
    REGSMCMD(L"br",m_text_cmdtarget);   //T
    REGSMCMD(L"textpic",m_text_cmdtarget);
    REGSMCMD(L"v",*this);
    REGSMCMD(L"bgm",*this); //T
    REGSMCMD(L"bgmstop",*this); //T
    REGSMCMD(L"se",*this);

    REGSMCMD(L"cgopen",*this);

    REGSMCMD(L"end",*this); //T

    REGSMCMD(L"snow_start",m_snow);     //T
    REGSMCMD(L"snow_setvol",m_snow);    //T
    REGSMCMD(L"snow_setwind",m_snow);   //T
    REGSMCMD(L"snow_setspeed",m_snow);  //T
    REGSMCMD(L"snow_stop",m_snow);  //T

    REGSMCMD(L"achr_m",m_chrs);   //Add a character with moving acting
	REGSMCMD(L"achr_ma",m_chrs);  //Add a character with moving acting and alpha changing
	REGSMCMD(L"chr",m_chrs);  //Add a character without any effect
	REGSMCMD(L"unchr",m_chrs);  //Clear a character
	REGSMCMD(L"chr_a",m_chrs);  //Animate a character with alpha changing
	REGSMCMD(L"chr_e",m_chrs);  //Effecting a character
	REGSMCMD(L"chr_m",m_chrs);  //Move a character
	REGSMCMD(L"chr_m2",m_chrs);  //Move a character with relative position
	REGSMCMD(L"chr_x",m_chrs);  //Exchange a character's with the other one
    REGSMCMD(L"chr_z",m_chrs);  //Zoom a character
    REGSMCMD(L"chr_zxy",m_chrs);  //Zoom a character with independent x,y

    REGSMCMD(L"bg_mask",m_mask);    //T
    REGSMCMD(L"bg",m_bg);
    REGSMCMD(L"bg_a",m_bg);
    REGSMCMD(L"bg_m",m_bg);
    REGSMCMD(L"bg_m2",m_bg);
    REGSMCMD(L"bg_m_cen",m_bg);
    REGSMCMD(L"bg_z",m_bg);
    REGSMCMD(L"bg_zxy",m_bg);
    REGSMCMD(L"setbgmode",m_bg);

}

void GalgameActivity::OnShow(){
    PNT("GALGAMEACTIVITY:ONSHOW");
    m_actGlbStat = SHOWING;
    m_actGlbTimer.Reset();
}

void GalgameActivity::OnInit()
{
    PNT("GALGAMEACTIVITY:ONINIT");

    m_name_font.Open(r.Str("AFX_FONT"));
    m_name.SetFont(&m_name_font);

    PNT("GALGAMEACTIVITY:ONINIT SNOW");
    m_snow.Init();
    PNT("GALGAMEACTIVITY:ONINIT CORNER");
    corner.Init();
    corner.SetFadeSpeed(600);
    corner.SetTopAlpha(192);
    //corner.SetSrc(0,0,64,74);
    //corner.SetPos(926,r.Int("GAL_CORNER_Y"));
    //corner.SetZoom(70,80);
    corner.SetRollCenter(128,147);

    PNT("GALGAMEACTIVITY:ONINIT twin");
    m_textWindow.Init();
    m_textWindow.Show();

    m_textWindow_X.SetNormalPic("GalGameSystem/window_normal_x1.png");
    m_textWindow_X.SetMotionPic("GalGameSystem/window_normal_x2.png");
    m_textWindow_X.SetDownPic("GalGameSystem/window_normal_x3.png");

    m_textWindow_save.SetNormalPic("GalGameSystem/window_normal_data1.png");
    m_textWindow_save.SetMotionPic("GalGameSystem/window_normal_data2.png");
    m_textWindow_save.SetDownPic("GalGameSystem/window_normal_data3.png");

    m_textWindow_set.SetNormalPic("GalGameSystem/window_normal_set1.png");
    m_textWindow_set.SetMotionPic("GalGameSystem/window_normal_set2.png");
    m_textWindow_set.SetDownPic("GalGameSystem/window_normal_set3.png");

    m_textWindow_skip.SetNormalPic("GalGameSystem/window_normal_skip1.png");
    m_textWindow_skip.SetMotionPic("GalGameSystem/window_normal_skip2.png");
    m_textWindow_skip.SetDownPic("GalGameSystem/window_normal_skip3.png");

    m_textWindow_auto.SetNormalPic("GalGameSystem/window_normal_auto1.png");
    m_textWindow_auto.SetMotionPic("GalGameSystem/window_normal_auto2.png");
    m_textWindow_auto.SetDownPic("GalGameSystem/window_normal_auto3.png");

    m_astat = RUNNING;
    PNT("GALGAMEACTIVITY:ONINIT END");
}

void GalgameActivity::OnHide()
{
    PNT("GALGAMEACTIVITY:ONHIDE");
    //m_autoSaver.SendMsg(-1);
    if(m_bgm.Playing()) m_bgm.Stop(250);
    if(m_cv.Playing()) m_cv.Stop(250);
    if(m_se.Playing()) m_se.Stop(250);

    gameData.UpdateData();
    pTextBox -> ForceClear();
    //m_autoSaver.WaitFinish();
    /*m_bg.Quit();
    m_chr.Quit(0);
    corner.Destory();
    m_snow.Stop();
    m_snow.Quit();
    m_textWindow.Quit();*/

    PNT("DELETE THIS");
    //delete pGal;
    //pGal = new GalgameActivity(pTextBox,pClock);
}

void GalgameActivity::OnNext()
{
    m_clock -> OnNext();
    m_bg.OnNext();
    m_mask.OnNext();
    m_chrs.OnNext();
    m_textWindow.OnNext();
    //m_textWindow_X.OnNext();
    corner.OnNext();
    m_text -> OnNext();
    m_name.OnNext();
    m_snow.OnNext();

        //PNT("IFSME");
    if(m_astat == RUNNING){
        SMENext();
    }

    if(m_autoUpdateDataTimer >= 5 * 60 * 1000){
        m_autoUpdateDataTimer.Reset();
        gameData.UpdateData();
        PNT("GAME  DATA AUTO UPDATED.");
    }
}

void GalgameActivity::OnDraw()
{
    SDL_SetRenderDrawColor(pRnd,255,255,255,255);
    pRnd.Clear();

    m_bg.OnDraw();
    m_mask.OnDraw();
    m_snow.OnDraw();
    m_chrs.OnDraw();

    m_textWindow.OnDraw();
    m_text -> OnDraw();
    m_textWindow_X.OnDraw();
    m_textWindow_save.OnDraw();
    m_textWindow_set.OnDraw();
    m_textWindow_skip.OnDraw();
    m_textWindow_auto.OnDraw();

    m_name.OnDraw();

    m_clock -> OnDraw();
    corner.OnDraw();

    if(m_actGlbStat == SHOWING){
        float per = float(m_actGlbTimer.GetTimer()) / 400;
        if(per>=1){
            m_actGlbStat = NOR;
        }else{
            SDL_SetRenderDrawColor(pRnd,0,0,0,255-255*per);
            SDL_SetRenderDrawBlendMode(pRnd,SDL_BLENDMODE_BLEND);
            pRnd.Clear();
        }
    }
}

void GalgameActivity::OnEvent(const SDL_Event& e)
{
    #ifdef _DEBUG
    if(e.type == SDL_MOUSEBUTTONDOWN) cout<<"MOUSE:"<<e.button.x<<","<<e.button.y<<endl;
    else if(e.type == SDL_KEYDOWN) cout<<"KEY:"<<SDL_GetKeyName(e.key.keysym.sym)<<endl;
    #endif
    if(e.type == SDL_MOUSEBUTTONUP && m_astat == WINDOWHIDDEN) ShowWindow();
    else if(e.type == SDL_MOUSEBUTTONUP){
        m_SMEProc_autoing = false;
        m_SMEProc_skipping = false;
        m_text -> SetEffectSpeed(r.Int("GAL_TEXTBOX_EFFECTSPEED"));
        if(e.button.button == SDL_BUTTON_LEFT) SMEClick();
        else if(e.button.button == SDL_BUTTON_RIGHT) HideWindow();
    }else if(e.type == SDL_KEYDOWN){
        std::string KeyName(SDL_GetKeyName(e.key.keysym.sym));
        if(KeyName == "Left Ctrl" || KeyName == "Right Ctrl"){
            m_SMEProc_skipping = true;
            m_text -> SetEffectSpeed(0);
            //cout<<"DOWN"<<endl;

        }
    }else if(e.type == SDL_KEYUP){
        std::string KeyName(SDL_GetKeyName(e.key.keysym.sym));
        if(KeyName == "Left Ctrl" || KeyName == "Right Ctrl"){
            m_SMEProc_skipping = false;
            m_text -> SetEffectSpeed(r.Int("GAL_TEXTBOX_EFFECTSPEED"));
            cout<<"UP"<<endl;
        }else if(KeyName == "Space"){
            if(m_astat == WINDOWHIDDEN) ShowWindow();
            else if(m_astat == RUNNING && m_SMEProc_jg.GetTimer() >= 10){
                m_SMEProc_autoing = false;
                m_SMEProc_skipping = false;
                m_text -> SetEffectSpeed(r.Int("GAL_TEXTBOX_EFFECTSPEED"));
                SMEClick();
                m_SMEProc_jg.Reset();
            }
        }else if(KeyName == "Down"){
            if(m_astat == WINDOWHIDDEN) ShowWindow();
            else if(m_astat == RUNNING && m_SMEProc_jg.GetTimer() >= 10) {
                    m_SMEProc_autoing = false;
                    m_SMEProc_skipping = false;
                    m_text -> SetEffectSpeed(r.Int("GAL_TEXTBOX_EFFECTSPEED"));
                    m_SMEProc_jg.Reset();
                    SMEClick();
            }
        }else if(KeyName == "D"){
                PNT("DEBUG:NO DEBUG CODE");
                //TODO:There write code for debug,and press key 'D' to run it in DEBUG ver and RC ver.
            }

            //FOR YANCHU
            #ifdef _DEBUG
            else if(KeyName == "F1"){
                if(m_SMEProc_stat != WAITCLICK) SMEClick();
                SaveGame(1);
                PNT("QUICK SAVE");
            }else if(KeyName == "F2"){
                if(m_SMEProc_stat != WAITCLICK) SMEClick();
                LoadSave(1);
                PNT("QUICK LOAD");
            }
            #endif
        }else if(e.type == SDL_MOUSEWHEEL){
        if(e.wheel.y<=-1){
            if(m_astat == WINDOWHIDDEN) ShowWindow();
            else if(m_astat == RUNNING && m_SMEProc_jg.GetTimer() >= 10) {
                m_SMEProc_autoing = false;
                m_SMEProc_skipping = false;
                m_text -> SetEffectSpeed(r.Int("GAL_TEXTBOX_EFFECTSPEED"));
                m_SMEProc_jg.Reset();
                SMEClick();
            }
        }
    }

    else if(e.type == SDL_MOUSEMOTION){
        if(e.motion.y>=r.Int("GAL_EVENT_CLOCKSHOW") && !m_clock -> GetVisible() && m_astat != WINDOWHIDDEN) m_clock -> Show();
        else if(e.motion.y < r.Int("GAL_EVENT_CLOCKSHOW") && m_clock -> GetVisible()) m_clock -> Hide();
    }


}

void GalgameActivity::OnEvent(Core::Control* c, const Sint32 msg)
{
    if(msg == 4){
        if(c == &m_textWindow_X) HideWindow();
        else if(c == &m_textWindow_set) Call(pSettingUI);
        else if(c == &m_textWindow_save) {((SaveUI*)pSaveUI) -> SetCallByTitle(false);Call(pSaveUI);}
        else if(c == &m_textWindow_skip){
            m_SMEProc_skipping = true;
            m_text -> SetEffectSpeed(0);
        }else if(c == &m_textWindow_auto){
            m_SMEProc_autoing = true;
            m_SMEProc_skipping = false;
            m_text -> SetEffectSpeed(r.Int("GAL_TEXTBOX_EFFECTSPEED"));
        }
    }
}

void GalgameActivity::SMEProc(SMI::SMEvent* pSme)
{
    if(pSme -> cmd == L"w"){
        m_waiter.Reset();
        m_waiterBroken = false;
    }else if(pSme -> cmd == L"v"){
        m_cv.Load("GalRes/Voice/"+WStringToString(pSme -> argv[0])+".ogg");
        m_cv.Volume(gameData.GetCVVol());
        m_cv.Play();
    }else if(pSme -> cmd == L"cgopen"){
        gameData.OpenCG(_wtoi(pSme -> argv[0].c_str()));
        gameData.AddUpdateTask(-1);
    }else if(pSme -> cmd == L"bgm"){
        m_bgm_name = WStringToString(pSme -> argv[0]);
        m_bgm_loader.Run();
        m_bgm_loader.SendData((void*)&m_bgm);
        m_bgm_loader.SendData((void*)&m_bgm_lock);
        m_bgm_loader.SendData((void*)&(pSme -> argv[0]));
        m_bgm_loader.WaitMsg(1);
    }else if(pSme -> cmd == L"bgmstop"){
        m_bgm_name.clear();
        m_bgm_lock.Lock();
        m_bgm.Stop(250);
        m_bgm_lock.Unlock();
    }else if(pSme -> cmd == L"se"){
        m_se.Load("GalRes/se/"+WStringToString(pSme -> argv[0])+".ogg");
        m_se.Volume(gameData.GetSEVol());
        m_se.Play();
    }else if(pSme -> cmd == L"end"){
        gameData.SetDataExist(0,false);
        gameData.AddUpdateTask(-2);
        gameData.UpdateData();
        Goto(pEd);
    }
}

void GalgameActivity::SMEFastProc(SMI::SMEvent* pSme)
{
    if(pSme -> cmd == L"w"){
        m_waiterBroken = true;
    }
}

bool GalgameActivity::SMEFinished(SMI::SMEvent* pSme)
{
    if(pSme -> cmd == L"w"){
        if(m_waiter.GetTimer() >= (unsigned int)(_wtoi(pSme ->argv[0].c_str())) || m_waiterBroken)
            return true;
        else return false;
    }

    return true;
}

void GalgameActivity::LoadSave(int num){
    m_bgm_name.clear();
    corner.Hide();
    if(num < 0){
        m_SMEProc_skipping = false;
        m_SMEProc_autoing = false;
        m_SMEProc_ProcCalled = false;
        m_SMEProc_FastProcCalled = false;
        m_name_isEmpty = true;
        m_snow.ForceStop();
        m_SMEProc_smi.Goto(L"start");
        m_SMEProc_stat = FINISHED;
        m_autoUpdateDataTimer.Reset();
        m_bg.Reset();
        m_chrs.Reset();
        //m_snow.ForceStop();
    }else{
        GameDataMgr::SAVE& saveBundle = gameData[num];
        saveBundle.ResetPtr();

        //Load BG
        m_bg.LoadData(saveBundle);

        //Load TextBox State TODO
        m_text -> ForceClear();
        /*TextBoxLog t;
        saveBundle.Read<SDL_Color>(t.defColor);
        saveBundle.Read<int>(t.defEffSpeed);
        saveBundle.Read<int>(t.defFontSize);
        saveBundle.Read<int>(t.defSpeed);

        int t_vecSize;
        saveBundle.Read<int>(t_vecSize);
        for(int i = 0;i < t_vecSize;++i){
            SMEvent e;
            saveBundle.Read<SMEType>(e.type);
            std::string temp;
            PNT("DDD1");
            saveBundle.ReadStr(temp);
            e.cmd = StringToWString(temp);
            Uint8 t_vecArgvSize;
            saveBundle.Read<Uint8>(t_vecArgvSize);
            PNT("t_vecSize"<<int(t_vecArgvSize));
            for(Uint8 j = 0;j < t_vecArgvSize;++j){
                saveBundle.ReadStr(temp);
                PNT("DDD2");
                e.argv.push_back(StringToWString(temp));
                PNT("DDD2:"<<temp);
            }
            t.Smes.push_back(e);
        }
        m_text_cmdtarget.LoadTextLogToText(t);

        //Load Name Card
        std::string nameCard;
        saveBundle.ReadStr(nameCard);
        if(nameCard  == "白雪") {
            m_name.SetColor(0xF9,0xC2,0xF8);
        }
        else if(nameCard  == "白峰") {
            m_name.SetColor(0x95,0xD1,0xF4);
        }
        m_name.SetText(StringToWString(nameCard));
        m_name_isEmpty = nameCard.empty();*/

        //Load BGM Music
        saveBundle.ReadStr(m_bgm_name);
        if(!m_bgm_name.empty()){
            std::wstring bgmName = StringToWString(m_bgm_name);
            m_bgm_loader.Run();
            m_bgm_loader.SendData((void*)&m_bgm);
            m_bgm_loader.SendData((void*)&m_bgm_lock);
            m_bgm_loader.SendData((void*)&bgmName);
            m_bgm_loader.WaitMsg(1);
        }

        //Load ChrMgr
        m_chrs.LoadData(saveBundle);

        //Load Snow
        m_snow.LoadState(saveBundle);

        //Load SMI States;
        m_SMEProc_smi.Load(saveBundle);

        //Finishing Load
        m_SMEProc_sme.type = SAYEND;
        m_SMEProc_stat = WAITCLICK;
        m_text -> ForceClear();
        SMEClick();
        textLog.clear();
        m_autoUpdateDataTimer.Reset();
    }
}

void GalgameActivity::SetVol(Uint8 bgm, Uint8 se, Uint8 cv)
{
    m_bgm_lock.Lock();
    m_bgm.Volume(bgm);
    m_bgm_lock.Unlock();
    m_cv.Volume(cv);
    m_se.Volume(se);
}

void GalgameActivity::LowerSaveGame()
{
    //Get Save Bundle
    GameDataMgr::SAVE& saveBundle = gameData[0];
    saveBundle.ResetPtr();

    //Save BG
    m_bg.SaveData(saveBundle);

    //Save TextBox State
    /*TextBoxLog& t = m_text_cmdtarget.GetTextLog();
    saveBundle.Write<SDL_Color>(t.defColor);
    saveBundle.Write<int>(t.defEffSpeed);
    saveBundle.Write<int>(t.defFontSize);
    saveBundle.Write<int>(t.defSpeed);
    saveBundle.Write<int>(t.Smes.size());
    FOR_EACH(p,t.Smes.begin(),t.Smes.end()){
        saveBundle.Write<SMEType>(p -> type);
        saveBundle.WriteStr(WStringToString(p -> cmd));
        saveBundle.Write<Uint8>(p -> argv.size());
        PNT("t_vecSize"<<p ->argv.size());
        FOR_EACH(pArg,p -> argv.begin(),p -> argv.end()){
            saveBundle.WriteStr(WStringToString(*pArg));
            PNT("DDD2"<<WStringToString(*pArg));
        }
    }*/

    //Save Name Card
    //saveBundle.WriteStr(WStringToString(m_name.GetText()));

    //Save BGM Music
    saveBundle.WriteStr(m_bgm_name);

    //Load ChrMgr
    m_chrs.SaveData(saveBundle);

    //Save Snow
    m_snow.SaveState(saveBundle);

    //Save SMI
    m_SMEProc_smi.Save(saveBundle);

    //Add Update Task To GameDataMgr
    gameData.AddUpdateTask(0);

    PNT("LOWSAVEGAME SAVESIZE:"<<saveBundle.WPtr());
}

void GalgameActivity::SaveGame(int num)
{
    memcpy(gameData[num],gameData[0],65536);
    gameData.AddUpdateTask(num);
    gameData.UpdateData();
}


GalgameActivity::~GalgameActivity()
{

}


void ACGCross::Galgame::BGMLoader(THREAD_ID id){
    Core::Sound* channel = (Core::Sound*) Core::GetData(id);
    Core::Mutex* lock = (Core::Mutex*) Core::GetData(id);
    std::wstring* name = (std::wstring*) Core::GetData(id);

    lock -> Lock();
    std::string nameAsc = WStringToString(*name);
    Core::ReturnMsg(id,1);
    channel -> Load("GalRes/bgm/"+nameAsc+".ogg");
    channel -> Volume(gameData.GetBGMVol());
    channel -> Play_Loop();
    lock -> Unlock();
}

