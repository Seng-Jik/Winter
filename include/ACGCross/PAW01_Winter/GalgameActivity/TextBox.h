#pragma once

#include "SMEProcUnit.h"
#include "ACGCross/PAW01_Winter/game.h"
#include <string>
#include <vector>
#include <list>
#include "Core.h"
#include "ACGCross/PAW01_Winter/str2wstr.h"

namespace ACGCross {
namespace Galgame {
class TextBox;

struct TextBoxLog{
    SDL_Color defColor;
    int defFontSize;
    int defSpeed,defEffSpeed;
    std::vector<SMI::SMEvent> Smes;
};

class TextBoxCmdTarget:public SMEProcUnit{
private:
    TextBox* m_text = nullptr;
    TextBoxLog m_log;

    int m_FontSize;
    SDL_Color m_Color;

public:
    void ClearTextLog();
    TextBoxLog& GetTextLog();
    void LoadTextLogToText(TextBoxLog& l);

    void SetText(TextBox*);
        //SME Interface
    void SMEProc(SMI::SMEvent* pSme);
    void SMEFastProc(SMI::SMEvent* pSme);
    bool SMEFinished(SMI::SMEvent* pSme);
};

class TextBox
{
    friend class TextBoxCmdTarget;
    public:
    enum Stat{
        NOEVENT,    //闲置
        TXT_SHOWING,    //文字显示中
        TXT_KILLING,    //文字消除中
        TXT_STOPSPEAK,  //强制显示文字中

        RECT_HIDING,    //正在全局隐藏
        RECT_SHOWING,   //正在全局显示
        RECT_HIDE   ////全局隐藏
    };
        TextBox();
        ~TextBox();

        //字体接口
        void SetEffectSpeed(int time);
        void SetColor(int r,int g,int b);
        void SetFontSize(int px = 32);

        //可以在途中插入，但是必须先换行再设大小，每一行只能用相同大小，除非你智商够高

        //文本接口
        void Br();  //换行
        void Clear();
        void AddText(const std::wstring&);  //追加文本
        void AddPic(const std::string& file,const int fps,const int time);   //追加表情图
        //文件名，文件中包含的帧数（横向），ms/帧 单位的播放速度

        //系统接口
        void OnNext();
        void OnDraw();
        inline bool Finished()
        {return m_stat == NOEVENT || m_stat == RECT_HIDE;};
        void Init();
        void Destroy();

        //对话接口
        void SetSpeed(int time);
        void StopSpeak();

        //文字框接口
        void SetRect(int x,int y,int w,int h);
        void Hide();
        void Show();
        Stat GetStatus(){return m_stat;};

        //强制添加文字接口，没有动画效果
        Uint32 ForceAddText(const std::wstring&);
        Uint32 ForceAddPic(const std::string& file,const int fps,const int time);
        //返回迭代器，该迭代器指向输入文本的第一文字的迭代器

        //std::vector<Core::Texture*>::iterator GetTextEnd()
        //{return m_text.end();}

        void ForceClear();  //强制删除文字



    protected:
    private:
        Core::ResFile m_rfont;
        TTF_Font* m_tfont = nullptr;
        std::vector<int> m_lineWord;    //每行文字数
        std::vector<Core::Texture*> m_text;  //文字
        SDL_Color m_color = {255,255,255,255};
        int
            m_fpsSpeed, //FPS/字 速度
            m_fpsEffectSpeed;   //FPS特效速度
        Stat m_stat; //状态
        int m_biggestLineHeight = 0;
        Uint32 m_nowFps;   //FPS计数器
        SDL_Rect m_rect;    //文字框位置
        int m_word_h;  //文字长度
        Uint16 m_showing_word; //显示中的文字
        Uint32 m_fpsCounter; //FPS的计数关键点

        int m_linePos;  //现在已经显示到的相对rect横行坐标
        int m_heiPos;  //现在已经显示到的相对rect竖行坐标

        struct TextPicInfo{
            int tex;
            int fpsCount;
            int nowFps;
            int fpsCounter;
            int fpsTime;
        };
        std::list<TextPicInfo> m_tpiv;
};

} // namespace Galgame
} // namespace ACGCross
