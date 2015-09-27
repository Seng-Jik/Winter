#include "ACGCross/PAW01_Winter/Game.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/TextBox.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/Clock.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalgameActivity.h"
#include "ACGCross/PAW01_Winter/Title/Title.h"
#include "ACGCross/PAW01_Winter/Ending.h"
#include "ACGCross/PAW01_Winter/SaveUI.h"
#include "ACGCross/PAW01_Winter/SettingUI.h"
#include "ACGCross/PAW01_Winter/CGViewer.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalSelButton.h"
using namespace std;
using namespace Core;
using namespace ACGCross;
using namespace ACGCross::Galgame;

GameDataMgr ACGCross::gameData;
ResVal ACGCross::r;

Core::Activity* ACGCross::pGal;
Core::Activity* ACGCross::pTitle;
Core::Activity* ACGCross::pEd;
Core::Activity* ACGCross::pSaveUI;
Core::Activity* ACGCross::pSettingUI;
Core::Activity* ACGCross::pCGViewer;
ACGCross::Galgame::TextBox* ACGCross::pTextBox;
ACGCross::Galgame::Clock* ACGCross::pClock;

void BugReporter(void*           userdata,
                           int             category,
                           SDL_LogPriority priority,
                           const char*     message){

    fstream error("crasherror.log",ios::app|ios::app);
    error<<"cate:"<<category<<"    message:"<<message<<endl;
    error.close();
}

void ACGCross::InitGame(){

    //LoadUI
    r.Load("ResVal/UI_BASE.txt");
    if(gameData.Get16_9()) r.Load("ResVal/UI_16_9.txt");

    //LoadLanguage
    LCID lcid = GetSystemDefaultLCID(); //GetSystemLanguage
    r.Load("ResVal/LNG_BASE_CHS.TXT");   //China WanSui!!
    switch(lcid){
    case  0x1004:
        r.Load("ResVal/LNG_CHT_MO.txt");break;
    case 3076:
        r.Load("ResVal/LNG_CHT_HK.txt");break;
    case 0x404:
        r.Load("ResVal/LNG_CHT_TW.txt");break;
    case 0x409:
        r.Load("ResVal/LNG_ENG.txt");break;
    case 0x411 :
        r.Load("ResVal/LNG_JPN.txt");break;
    }

    //Init Class Static Member Value
    static ResFile selBk[3];
    selBk[0].Load("GalGameSystem/selBk1.png");
    GalSelButton::m_btnbk[0] = IMG_LoadTexture_RW(pRnd,selBk[0],selBk[0].Size());
    selBk[1].Load("GalGameSystem/selBk2.png");
    GalSelButton::m_btnbk[1] = IMG_LoadTexture_RW(pRnd,selBk[1],selBk[1].Size());
    selBk[2].Load("GalGameSystem/selBk3.png");
    GalSelButton::m_btnbk[2] = IMG_LoadTexture_RW(pRnd,selBk[2],selBk[2].Size());

    //Load Font

    SDL_LogSetOutputFunction(BugReporter, nullptr);
    pTextBox = new Galgame::TextBox;
    pClock = new Galgame::Clock;
    pTextBox -> Init();
    pClock -> Init();
    pClock -> SetPos(20,20);

    pGal = new Galgame::GalgameActivity(pTextBox,pClock);
    pTitle = new Galgame::Title;
    pEd = new Ending;
    pSaveUI = new SaveUI;
    pSettingUI = new SettingUI;
    pCGViewer = new Galgame::CGViewer;


    /*SDL_LogOutputFunction* pReporter = nullptr;
    SDL_LogGetOutputFunction(pReporter,nullptr);
    (void (*)(void*,int,SDL_LogPriority,const char*))pReporter(nullptr,0,SDL_LOG_CATEGORY_APPLICATION,"Log works OK!");
    //TTF_SetFontOutline(*pFont,1);*/
}

void ACGCross::QuitGame(){
    //delete pTitle;
    //delete pGal;
    delete pEd;
    //delete pFont;
}
