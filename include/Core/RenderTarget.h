#ifndef _HEAD_RENDERTARGER_HEAD_
#define _HEAD_RENDERTARGER_HEAD_
#include "Core/Thread.h"
namespace Core{
class RenderTarget  //你可以把本应绘制到屏幕缓存的数据绘制到这个类的对象里
{
    public:
        RenderTarget(int w,int h);  //传入长,宽以创建绘制缓存
        ~RenderTarget();
        static bool Supposed(); //检查是否支持使用此类，如果不支持，请不要使用此类

        void SetDrawPos(int x,int y,int w,int h);  //设置绘制到屏幕上时用到的位置
        void SetDrawArea(int x,int y,int w,int h);  //设置绘制到屏幕上时剪裁的部分

        void DrawToScreen();    //将自己的数据复制到屏幕缓存上，执行前一定要执行End()。

        void Begin();   //执行此语句后，本应绘制到屏幕的数据将会全部被绘制到此对象里
        static void End();  //完成对本对象的绘制后，将绘制目标设回屏幕缓存，但不会把本对象内的数据复制到屏幕缓存

        operator SDL_Texture* ();   //获取纹理以自己定义复制到屏幕缓存方式，请不要用Core::Texture管理它，也不要销毁它
    protected:
    private:
        SDL_Texture* m_target;
        SDL_Rect m_drawArea,m_drawPos;
};

class SoftLinkTarget{ //你可以绑定其他的软件渲染器到本类对象，比如用SALVIA进行3D渲染或者用ffmpeg播放视频
    public:
        SoftLinkTarget(int w,int h,Uint32 sdlPixelFormat);  //传入长,宽以及SDL支持的像素格式

        void SetMutex(Mutex*);  //使用此函数后，会在DrawToScreen时锁死

        operator void* ();  //返回帧缓存位置
        int Pitch();    //返回一行大小

        void Update();  //绑定的渲染器完成渲染后执行此操作，将更改应用到显存
        void DrawToScreen();    //绘制到屏幕

        void SetDrawPos(int x,int y,int w,int h);  //设置绘制到屏幕上时用到的位置
        void SetDrawArea(int x,int y,int w,int h);  //设置绘制到屏幕上时剪裁的部分

        operator SDL_Texture* ();   //获取纹理以自定义绘制到屏幕上的方式，请不要用Core::Texture管理它，或者销毁它
    private:
        SDL_Rect m_drawArea,m_drawPos;
        void* m_frameBuf;
        int m_pitch;
        SDL_Texture* m_target;
        Mutex* m_mut;
};
}

#endif // _HEAD_RENDERTARGER_HEAD_
