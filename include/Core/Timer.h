#ifndef _HEAD_TIMER_
#define _HEAD_TIMER_
#include "../Core/Debug.h"

/***** 计时器 *****
* 类似NS的那种方法。
* 每一个对象都可以是独立的计时器，但是它们使用共同的底层计时器。
* 原理是使用时间差。
* Reset重置Timer为0。
* GetTimer返回当前的时间值（毫秒数）。
* WaitTimer用来使用SDL_Delay等待计时器跑到一定时间。
* 2014.8.24
******************/

#include "SDLbase.h"
#include "Globals.h"
namespace Core
{
    class Timer
    {
    private:
        Uint32 m_timeStartPos;
    public:
        inline Timer();
        inline void Reset();
        inline Uint32 GetTimer();
        inline void WaitTimer(Uint32);
        inline operator Uint32 ();
    };

    class FPSTimer{
    private:
         Uint32 m_tck;
    public:
        inline FPSTimer(){
            m_tck = pRnd.GetFpsCount();
        }
        inline void Reset(){
            m_tck = pRnd.GetFpsCount();
        }
        inline Uint32 GetTimer(){
            return pRnd.GetFpsCount() - m_tck;
        }
        inline operator Uint32 (){
            return GetTimer();
        }
    };


    Timer::Timer(void)
    {
        Reset();
    }

    void Timer::Reset()
    {
        m_timeStartPos = SDL_GetTicks();
    }

    Uint32 Timer::GetTimer()
    {
        return SDL_GetTicks() - m_timeStartPos;
    }

    void Timer::WaitTimer(Uint32 _time)
    {
        Uint32 t = GetTimer();
        if (_time < t) return;
        else SDL_Delay(_time - t);
    }
    Timer::operator Uint32 (){
        return GetTimer();
    }
}




#endif // _HEAD_TIMER_
