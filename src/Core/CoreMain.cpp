#include "Core/Activity.h"
#include "Core/Timer.h"
#include "Core/Sound.h"
#include "Core/Globals.h"
#include "Core/Control.h"
#include "Core/Debug.h"
#include <stack>

using namespace Core;
using namespace std;



namespace Core{

RndPtr pRnd;
Activity* nowFocus;
Activity* nextFocus;
stack<Activity*> actStack;
bool jumpDraw = false;

void JumpDraw(){
    jumpDraw = true;
}
void Goto(Activity* a)
{
    if(a != nowFocus)
        nextFocus = a;
}

void Call(Activity* a){
    actStack.push(nowFocus);
    nowFocus = a;
    a -> OnShow();
}
//Activity* GetParent()
//{return actStack.top();}

void Return(){
    nowFocus -> OnHide();
    nowFocus = actStack.top();
    actStack.pop();
}

//void ActivityDrawProc() //活动刷新一次处理
//{
    /*  取消活动间跳转动画功能   *
    if(isGotoing){  //如果正在跳转
        bool lastFinished = false;
        bool nowFinished = false;

        if(lastFocus != nullptr && !lastFinished) {
            if(lastFocus -> GetAnimationOnHide() != nullptr){
                lastFocus -> GetAnimationOnHide() -> OnNext();   //绘制上个活动
                if(lastFocus -> GetAnimationOnHide() -> Finished()) lastFinished = true;
            }else lastFinished = true;
            lastFocus -> OnDraw();
        }else lastFinished = true;

        if(nowFocus != nullptr && !nowFinished) {
            if(nowFocus -> GetAnimationOnShow() != nullptr){
                nowFocus -> GetAnimationOnShow() -> OnNext();   //绘制下个活动
                if(nowFocus -> GetAnimationOnShow() -> Finished()) nowFinished = true;
            }else nowFinished = true;
            nowFocus -> OnDraw();
        }else nowFinished = true;

        if(lastFinished && nowFinished) isGotoing = false;
    }

    //两个动画结束后关闭跳转状态
    else{
        */
        //nowFocus -> OnNext();
        //nowFocus -> OnDraw();
    //}
//}

void Exit(int exitcode)
{
    if(nowFocus != nullptr) nowFocus -> OnHide();
    while(!actStack.empty()){
        Activity*p = actStack.top();
        p -> OnHide();
        actStack.pop();
    }
    exit(exitcode);
}

void CoreRun(Activity* start,const string& title,const bool fullScreen,const int w,const int h)
{
    pRnd.Create(title,fullScreen,w,h);
    SDL_SetRenderDrawBlendMode(pRnd,SDL_BLENDMODE_BLEND);

    nowFocus = nullptr;
    nextFocus = start;
    //nowFocus -> OnShow();
    /* 主循环部分 */
    SDL_Event e;
    Timer FPSKiller;

    #ifdef _DEBUG
    Timer FPSCounter;
    int fps = 0;
    #endif // _DEBUG

    while(1){
        #ifdef _DEBUG
        fps++;
        if(FPSCounter.GetTimer()>=1000)
        {
            PNT("FPS:"<<fps);
            fps = 0;
            FPSCounter.Reset();
        }
        #endif // _DEBUG

        /**** 如果有Goto消息则执行Goto ****/
        if(nextFocus != nullptr){
                if(nowFocus != nullptr) nowFocus -> OnHide();
                while(!actStack.empty()){
                    Activity*p = actStack.top();
                    p -> OnHide();
                    actStack.pop();
                }
                nowFocus = nextFocus;
                nextFocus = nullptr;
                SDL_RenderSetLogicalSize(pRnd,nowFocus -> m_logic_w,nowFocus -> m_logic_h);
                if(!(nowFocus -> m_Inited)){
                    nowFocus ->OnInit();
                    nowFocus ->m_Inited = true;
                }
                nowFocus -> OnShow();
        }

        /**** 处理当前帧所有的事件 ****/
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT) {   //如果是退出
                nowFocus -> OnHide();
                while(!actStack.empty()){
                    Activity*p = actStack.top();
                    p -> OnHide();
                    actStack.pop();
                }
                pRnd.Destory();
                return;
            }
            else {
                if(!(nowFocus -> m_ansList.empty())){
                    bool SendToActivity = true;
                    FOR_EACH(p,nowFocus -> m_ansList.begin(),nowFocus -> m_ansList.end())  //遍历控件表
                    {
                        if((*p) -> OnEvent(e,*nowFocus)){  //发现有控件接受该信息后返回
                            SendToActivity = false;
                            break;
                        }
                    }
                    if(SendToActivity) nowFocus -> OnEvent(e);    //无控件接受消息，发送消息给活动的OnEvent()
                }else nowFocus -> OnEvent(e);
            }
        }

        /**** 绘制 ****/
        SDL_SetRenderDrawColor(pRnd,0x00,0x00,0x00,0xFF);
        nowFocus -> OnNext();   //可以取得上一帧数据
        if(jumpDraw) {
            pRnd.m_fps++;
            jumpDraw = false;
            continue;
        }
        SDL_RenderClear(pRnd);
        nowFocus -> OnDraw();
        SDL_RenderPresent(pRnd);

        FPSKiller.WaitTimer(1000/FPS);   //FPS限制
        FPSKiller.Reset();
        pRnd.m_fps++;
    }
}

int RndPtr::GetH()
{
    return nowFocus -> m_logic_h;
}

int RndPtr::GetW()
{
    return nowFocus -> m_logic_w;
}

int main(int argc,char** argv)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    Sound::Init();

    nowFocus = nullptr;
    nextFocus = nullptr;

    std::vector<std::string> args;
    for(int i = 0;i < argc;++i)
        args.push_back(std::string(argv[i]));

    Main(args);

    return 0;

}

}
