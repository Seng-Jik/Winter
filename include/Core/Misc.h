#ifndef _HEAD_MISC_
#define _HEAD_MISC_
#include <fstream>
#include "Core/SDLbase.h"
#include "Core/ResFile.h"
struct SDL_Rect;
namespace Core
{
    void DrawTextOutLine(SDL_Surface*); //字体描边
    void GetString(std::ifstream&,std::string&);    //从二进制文件流中取出一个字符串，遇到\0结尾
    void UpperCase(std::string&);
    std::string GetLine(ResFile& r,Uint32& pos);

    void Pos(int& opr_x,int& opr_y,float x,float y);    //使用百分比相对定位
    void Pos(int& opr_x,int& opr_y,const char base,int x,int y);    //使用基点相对定位
        /* 基点(Base)的用法 */
            //base是一个16进制数
            //把屏幕纵横切5刀，base的第一位是x位置
            //如0x00为最左边
            //0x20为中间
            //0x40为右边
            //第二位是纵向位置
            //0x00是最上边
            //0x02是中间
            //0x04是最下边
            //两个位置组合起来形成基位置，后边的x和y会相对于基位置进行计算
            //特殊位置：
            //A和B也是有效值，A为三分之一，B为三分之二

    bool InRect(const SDL_Rect&,int x,int y);
    Uint32 Time2Fps(Uint32 ms);
    void FastBlurARGB8888(SDL_Surface* pOrg,int radius);    //快速模糊
}


#endif
