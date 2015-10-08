#pragma once
#include "../NumTexture.h"
namespace ACGCross{
namespace Galgame{

class Clock{
private:
    ::Snow::Sprite m_fg;
    NumSprite m_clk;

    int m_bx;

    int m_x;
    int m_y;
    int m_numWei;
    int m_fgWei;

    int m_stat;

    bool m_visible;
    bool m_visible_l;

    char m_sz [4];
    bool m_fg_showing;
    float m_fg_last = 0.0;
    ::Snow::Timer m_timer_fg;
public:
    Clock();
    void OnNext();
    void OnDraw();
    void Init();  //必须先Init，否则可能出错
    void Destory();
    void Hide();
    void Show();
    void SetPos(int x,int y);
    bool GetVisible(){return m_visible_l;}
};

}
}
