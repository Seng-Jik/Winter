#pragma once

#include "SDLbase.h"

#define ActivityEvent(SINT32_EVENT) a.OnEvent((Control*)this,SINT32_EVENT);

/* 控件基类介绍 */
/*
* 使用Activity的RegControl注册控件到活动中，在活动处理事件之前会要求每个注册的Control处理事件。
* 控件可以截断事件和向活动发送事件以提高封装性。
*/
namespace Snow{
    class Activity;
    class Control   //控件基类
    {
    public:
        virtual bool OnEvent (const SDL_Event&,Activity& a) =0;
        //回调函数,处理消息，如果该控件截断此消息，则返回true，否则返回false
    };
}

