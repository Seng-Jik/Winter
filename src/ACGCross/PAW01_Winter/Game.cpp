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
using namespace ::Snow;
using namespace ACGCross;
using namespace ACGCross::Galgame;

GameDataMgr ACGCross::gameData;
ResVal ACGCross::r;

::Snow::Activity* ACGCross::pGal;
::Snow::Activity* ACGCross::pTitle;
::Snow::Activity* ACGCross::pEd;
::Snow::Activity* ACGCross::pSaveUI;
::Snow::Activity* ACGCross::pSettingUI;
::Snow::Activity* ACGCross::pCGViewer;
ACGCross::Galgame::TextBox* ACGCross::pTextBox;
ACGCross::Galgame::Clock* ACGCross::pClock;
ACGCross::MousePointer* ACGCross::pMouse;

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
    r.Load("ResVal/LNG_BASE_CHS.TXT");
    if(lcid == 0x1004 || lcid == 3076 || lcid == 0x404)
        r.Load("ResVal/LNG_CHT.txt");

    //Init Class Static Member Value
    static ResFile selBk[3];
    selBk[0].Load("GalGameSystem/selBk1.png");
    GalSelButton::m_btnbk[0] = IMG_LoadTexture_RW(pRnd,selBk[0],selBk[0].Size());
    selBk[1].Load("GalGameSystem/selBk2.png");
    GalSelButton::m_btnbk[1] = IMG_LoadTexture_RW(pRnd,selBk[1],selBk[1].Size());
    selBk[2].Load("GalGameSystem/selBk3.png");
    GalSelButton::m_btnbk[2] = IMG_LoadTexture_RW(pRnd,selBk[2],selBk[2].Size());

    //Load Mouse
    pMouse = new MousePointer;
    pMouse -> Load("GalGameSystem/mouse.png");


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
