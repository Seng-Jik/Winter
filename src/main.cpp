#include "Core.h"
#include "ACGCross/ACGCross_Logo.h"
#include "ACGCross/GalgameActivity/GalgameActivity.h"
#include "ACGCross/Title/Title.h"

#include "ACGCross/Game.h"
#include "Core/ResVal.h"
#include <stdlib.h>
#include <time.h>
#include "ACGCross/Ending.h"

#define GAME_TITLE "冬"

//#ifdef _PLATFORM_WINDOWS
//#include
//#endif // _PLATFORM_WINDOWS

using namespace std;
using namespace Core;
using namespace ACGCross;

struct screen{
    int w,h;
    bool full;
};

#include "Mask.h"
class TestActivity:public Core::Activity{
private:
    Timer m_t;
    Surface m_sur;
    Mask m_m;
    Texture m_tex;
public:
    TestActivity(){
        m_sur.Load("bk.png");
        m_sur.ConvertFormat(SDL_PIXELFORMAT_ARGB8888);
        m_m.LoadMaskPic("msk.png");
    }
    void OnShow(){
        m_t.Reset();
    }
    void OnDraw(){
        float per = float(m_t.GetTimer()) / 500;
        if(per < 1.0 && per >=0){
            m_m.ApplyToSurfaceARGB8888(m_sur,per);
            m_tex.Load(m_sur);
            m_tex.SetPos(0,0);m_tex.SetZoom(pRnd.GetW(),pRnd.GetH());
        }else if(per >= 1.0){
            m_m.ApplyToSurfaceARGB8888(m_sur,1);
            m_tex.Load(m_sur);
            m_tex.SetPos(0,0);m_tex.SetZoom(pRnd.GetW(),pRnd.GetH());
        }
        m_tex.OnDraw();
        //m_m.DrawDEBUG();
    }
    void OnInit(){

    }
    void OnEvent(const SDL_Event& e){
        //if(e.type == SDL_MOUSEWHEEL)
            //Goto(pTitle);
    }
};

#include "ACGCross/DanmakuGame/DanmakuGame.h"


void Main(const std::vector<std::string> args)
{
    srand(time(NULL));
    screen screenmode[] ={
        {800,600,false},
        {1024,768,false},
        {1280,960,false},
        {1024,768,true},
        {1280,960,true},
        {-1,-1,true},
        {800,450,false},
        {1024,576,false},
        {1280,720,false},
        {1024,576,true},
        {1280,720,true},
        {-1,-1,true}
    };
    pRnd.Create("冬",screenmode[gameData.GetGrpMode()].full,
            screenmode[gameData.GetGrpMode()].w,screenmode[gameData.GetGrpMode()].h);
    InitGame();

    #ifdef _DEBUG
    //TestActivity t;


        if(SDL_NumJoysticks()){ //如果使用手柄
            SDL_Joystick* pJoy = SDL_JoystickOpen(0);
            SDL_JoystickID id = SDL_JoystickInstanceID(pJoy);
            KeyMapActivity::SetJoyKeyMap(id,0,GKEY_UP);
            KeyMapActivity::SetJoyKeyMap(id,2,GKEY_DOWN);
            KeyMapActivity::SetJoyKeyMap(id,3,GKEY_LEFT);
            KeyMapActivity::SetJoyKeyMap(id,1,GKEY_RIGHT);
            KeyMapActivity::SetJoyKeyMap(id,7,GKEY_SLOW);
        }else{  //不使用手柄
            KeyMapActivity::SetKeyboardMap(SDLK_w,GKEY_UP);
            KeyMapActivity::SetKeyboardMap(SDLK_s,GKEY_DOWN);
            KeyMapActivity::SetKeyboardMap(SDLK_a,GKEY_LEFT);
            KeyMapActivity::SetKeyboardMap(SDLK_d,GKEY_RIGHT);
            KeyMapActivity::SetKeyboardMap(SDLK_LSHIFT,GKEY_SLOW);
        }
            //screenmode[gameData.GetGrpMode()].w,
            //screenmode[gameData.GetGrpMode()].h);
        //CoreRun(new DanmakuGame);  //启动弹幕游戏活动
        CoreRun(pGal);   //启动Galgame活动
        #else

        ACGCross::Logo l;
        l.SetGoto(*pTitle);
        CoreRun(&l,"冬",screenmode[gameData.GetGrpMode()].full,
             screenmode[gameData.GetGrpMode()].w,
             screenmode[gameData.GetGrpMode()].h);
        #endif // _DEBUG
        exit(0);
}
