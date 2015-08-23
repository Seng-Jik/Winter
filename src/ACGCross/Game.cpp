#include "Game.h"
#include "TextBox.h"
#include "Clock.h"
#include "ACGCross/GalgameActivity/GalgameActivity.h"
#include "ACGCross/Title/Title.h"
#include "ACGCross/Ending.h"
#include "ACGCross/SaveUI.h"
#include "ACGCross/SettingUI.h"
using namespace std;
using namespace Core;
using namespace ACGCross;

GameDataMgr ACGCross::gameData;
ResVal ACGCross::r;

Core::Activity* ACGCross::pGal;
Core::Activity* ACGCross::pTitle;
Core::Activity* ACGCross::pEd;
Core::Activity* ACGCross::pSaveUI;
Core::Activity* ACGCross::pSettingUI;
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
