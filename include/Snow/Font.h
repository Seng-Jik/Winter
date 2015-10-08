#pragma once

#include "SDLbase.h"
#include <string>
#include "ResFile.h"

/* 字体类，你只需要创建其对象加载字体即可 */
namespace Snow
{
    class Font
    {
    private:
        TTF_Font* m_font;
        ResFile m_file;
    public:
        Font();
        Font(const std::string&,int pt = 32);
        void Open(const std::string&,int pt = 32);
        operator TTF_Font* ();
        //void SetStyle(const int);
        //TTF_STYLE_BOLD
        //TTF_STYLE_ITALIC
        //TTF_STYLE_UNDERLINE
        //TTF_STYLE_STRIKETHROUGH

        //void SetOutLine(const int);
        //void SetHinting(const int);
        //void SetKerning(const int);
        int GetCharWei(const char); //取得一个ASCII字符的宽度
        int GetCharWei(const wchar_t);
        ~Font();
    };
}

