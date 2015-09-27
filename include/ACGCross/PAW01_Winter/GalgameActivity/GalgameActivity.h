#pragma once
#include "Core.h"

//#include "TextBox.h"
#include "Clock.h"
//#include "GameMenu.h"
#include "TextBox.h"
#include "NameCard.h"
//#include "SettingUI.h"
#include "Corner.h"
#include "Snow.h"
#include "Bg.h"
#include "../../../SMLanguage/SMI.h"
#include "TextWindow.h"
#include "MaskCmdTarget.h"
#include "../Button.h"
#include "ChrMgr.h"
#include "SMEProcUnit.h"
#include <map>
#include <stack>
#include <list>
#include "GalSelButton.h"

namespace ACGCross{
namespace Galgame{

void BGMLoader(Core::THREAD_ID id);
// I need three data
//a pointer to m_bgm
//a pointer to m_bgm_lock
//and a pointer to string of file basic name.

class GalgameActivity:public Core::Activity,SMEProcUnit{
    friend class TextWindow;
private:
    std::list<GalSelButton> m_sels;
    std::vector<std::wstring> m_selGoto;

    class TextWindowButton:public ACGCross::Button,public TextWindow::Widget{
    public:
        virtual void TWPos(int x,int y){
            Button::SetPos(x,y);
        };
        virtual void TWAlpha(Uint8 a){
            Button::SetAlpha(a);
            if(a == 0) Hide();
            else Show();
        };
    };

    enum {SHOWING,NOR} m_actGlbStat = NOR;
    Core::Timer m_actGlbTimer;

    enum ActState{
        RUNNING,
        WINDOWHIDDEN
    } m_astat = RUNNING;

    friend class TextBox;
    TextBox* m_text = nullptr;
    TextBoxCmdTarget m_text_cmdtarget;
    NameCard m_name;
    bool m_name_isEmpty = true;    //Current text has namecard?
    TextWindow m_textWindow;
    TextWindowButton m_textWindow_X;
    TextWindowButton m_textWindow_save;
    TextWindowButton m_textWindow_set;
    TextWindowButton m_textWindow_skip;
    TextWindowButton m_textWindow_auto;
    //Clock m_clk;
    //Snow m_snow;
    //
    //Bg m_bg;wow

    void SetWindowStyle_Normal();

    //SME Processor
    enum SMEProcessorState{
        PROCESSING, //Processing a sme
        FINISHED,   //Processed this sme
        WAITCLICK,    //Wait a click for game
        END   //SME END
    } m_SMEProc_stat = FINISHED;
    SMI::Interpreter m_SMEProc_smi;
    SMI::SMEvent m_SMEProc_sme;

    std::map<std::wstring,SMEProcUnit*> m_SMEProc_map;

    SMEProcUnit* m_SMEProc_Say;
    SMEProcUnit* m_SMEProc_Name;

    bool m_SMEProc_ProcCalled = false;
    bool m_SMEProc_FastProcCalled = false;

    void SMEClick(); //When clicked screen,call this.
    void SMENext(); //Call it when OnNext was called.
    void SMEUpdateStat();   //Update stat
    void SMEProc(bool fast = false);    //Process sme
    bool m_SMEProc_skipping = false;
    bool m_SMEProc_autoing = false;
    Core::Timer m_SMEProc_autoTimer;
    int m_SMEProc_skipFPSClicks = 0;
    Core::FPSTimer m_SMEProc_jg;
    Core::Timer m_waiter;
    bool m_waiterBroken;

    void SMEText_lb();

    void HideWindow();
    void ShowWindow();

    Core::Sound m_cv;
    std::string m_cvstr;
    Core::Sound m_se;
    Core::Sound m_bgm;  //访问加锁，m_bgm_lock
    std::string m_bgm_name;
    Core::Mutex m_bgm_lock;
    Core::Thread m_bgm_loader;

    friend class Bg;
    friend class ChrMgr;
    MaskCmdTarget m_mask;
    Bg m_bg;
    ChrMgr m_chrs;

    Snow m_snow;

    Clock* m_clock;

    Core::Timer m_autoUpdateDataTimer;

    void LowerSaveGame();

public:
    Core::Font m_name_font;
    Corner corner;
    static std::list<TextBoxLog> textLog;
    static std::list<std::string> cvLog;
    Uint8* saveScreen;
    Uint8* saveScreen_org;
    ~GalgameActivity();

    void RefreshSaveScreen();
    void LoadSave(int num); //if num == -1 reset SMI to restart game.
    void SaveGame(int num); //Save game states;
    SMI::SMEvent* GetSme(){return &m_SMEProc_sme;}
    GalgameActivity(TextBox* b,Clock* c);
    void OnInit();
    void OnShow();
    void OnHide();
    void OnNext();
    void OnDraw();
    void OnEvent(const SDL_Event&);
    void OnEvent(Core::Control*,const Sint32);
    virtual void SetVol(Uint8 bgm,Uint8 se,Uint8 cv);

    virtual void SMEProc(SMI::SMEvent*);
    virtual void SMEFastProc(SMI::SMEvent*);
    virtual bool SMEFinished(SMI::SMEvent*);


};

}
}
