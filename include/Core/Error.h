#ifndef _HEAD_ERROR_
#define _HEAD_ERROR_
#include "SDLbase.h"
#include <string>

#include "OS.h"
#define CATCH_BADALLOC catch(bad_alloc){Error(0x30000000,"内存不足");}
namespace Core{
    class Error{
    private:
        //错误级别：
        //0-不是错误
        //1-无影响错误,PNT宏输出
        //2-错误,PNT输出,弹出错误框
        //3-致命错误,PNT输出,弹出错误框,闪退
        //4-毁灭性错误,强制闪退
        Uint32 m_code;  //错误代码，用来查询错误修正方案
        //第一位hex是错误级别

        std::string m_msg;
    protected:
        std::string GetMsg();
        Uint8 GetLevel();
        void Proc();    //处理错误
        void Print();
    public:
        Error(Uint32,std::string);
    };
}

#endif // _HEAD_ERROR_
