#ifndef _HEAD_CONTROL_
#define _HEAD_CONTROL_

#include "SDLbase.h"

#define ActivityEvent(UINT32_EVENT) a.OnEvent((Control*)this,UINT32_EVENT);

/* 控件基类介绍 */
/*
控件基类应当与View并用
当操作时，如果鼠标、触摸在当前控件焦掉框中
则调用Proc函数响应基本操作，然后再调用活动中的Proc函数进一步操作。
*/
namespace Core{
    class Activity;
    class Control   //控件基类
    {
    public:
        virtual bool OnEvent (const SDL_Event&,Activity& a) =0;
        //回调函数,处理消息，如果该控件接受此消息，则返回true，否则返回false
    };
}

#endif // _HEAD_CONTROL_
