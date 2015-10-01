#ifndef _HEAD_RNDPTR_HEAD_
#define _HEAD_RNDPTR_HEAD_
#include "SDLbase.h"
#include <string>

namespace Core{
    class Activity;
    class RndPtr{   //渲染器和窗口智能指针,请不要创建它的对象，请使用pRnd对象
    friend void Core::CoreRun(Activity* start);
    private:
        SDL_Window* m_wnd;
        SDL_Renderer* m_rnd;
        Uint64 m_fps;
        int m_w,m_h;
    public:
        void Create(const std::string& title,const bool FullScreen,const int w,const int h);
        void Destory();
        void Clear();
        int GetW(); //逻辑宽
        int GetH(); //逻辑高
        inline int GetPhW(){return m_w;}    //物理宽
        inline int GetPhH(){return m_h;}    //物理高
        inline Uint64 GetFpsCount(){return m_fps;};   //取已经过的fps数
        inline operator SDL_Window* (){return m_wnd;};
        inline operator SDL_Renderer* (){return m_rnd;};
        SDL_Texture* GetRenderTarget();
        bool RenderTargetSupported();
    };
    extern RndPtr pRnd;
}
#endif
