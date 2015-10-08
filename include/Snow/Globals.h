#pragma once
#define FPS 60

#define FOR_EACH(val,begin,end) for(auto val = begin;val != end;++val)
#include "SDLbase.h"
#include <vector>
#include <string>
#include "Sound.h"

namespace Snow{
    class Activity;
    void Init();    //初始化
    void Run(Activity* start);  //框架执行
    //初始化活动的代码块的最后一行调用此函数，传入第一个执行的活动即可

    void Goto(Activity*);   //跳转活动
    void Call(Activity*);  //调用活动
    void Return();  //活动调用返回
    void JumpDraw();    //不渲染当前帧
    void Exit(int exitcode = 0);    //安全退出
}

typedef unsigned char BYTE;
