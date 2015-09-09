#ifndef _HEAD_GLOBALS_
#define _HEAD_GLOBALS_

#define FPS 60

#define FOR_EACH(val,begin,end) for(auto val = begin;val != end;++val)
#include "SDLbase.h"
#include <vector>
#include <string>
#include "../Core/Debug.h"

void Main(const std::vector<std::string>& args);

namespace Core{
    class Activity;
    class RndPtr;

    extern RndPtr pRnd;
    void CoreRun(Activity* start,const std::string& title,const bool fullScreen,const int w,const int h);  //框架执行
    //初始化活动的代码块的最后一行调用此函数，传入第一个执行的活动即可

    void Goto(Activity*);   //跳转活动
    void Call(Activity*);  //调用活动
    void Return();  //活动调用返回
    void JumpDraw();    //不渲染当前帧
    void Exit(int exitcode = 0);    //安全退出
    //Activity* GetParent();  //获取父活动

    class RndPtr{   //渲染器和窗口智能指针
    friend void CoreRun(Activity* start,const std::string& title,const bool fullScreen,const int w,const int h);
    private:
        SDL_Window* m_wnd;
        SDL_Renderer* m_rnd;
        Uint64 m_fps;
        int m_w,m_h;

        void Create(const std::string& title,const bool FullScreen,const int w,const int h);
        void Destory();
    public:
        void Clear();
        int GetW(); //逻辑宽
        int GetH(); //逻辑高
        inline int GetPhW(){return m_w;}    //物理宽
        inline int GetPhH(){return m_h;}    //物理高
        inline Uint64 GetFpsCount(){return m_fps;};   //取已经过的fps数
        inline operator SDL_Window* (){return m_wnd;};
        inline operator SDL_Renderer* (){return m_rnd;};
        void SetRenderTarget(SDL_Texture*);
        SDL_Texture* GetRenderTarget();
        bool RenderTargetSupported();
    };
}

typedef unsigned char BYTE;


#include "OS.h"
#endif
