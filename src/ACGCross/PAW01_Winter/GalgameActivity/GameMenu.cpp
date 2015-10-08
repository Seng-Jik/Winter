#include "ACGCross/PAW01_Winter/GalgameActivity/GameMenu.h"
#include "Snow.h"
#include "ACGCross/PAW01_Winter/NumTexture.h"

using namespace ::Snow;
using namespace std;
using namespace ACGCross::Galgame;

/* m_stat 状态
0 隐藏
1 普通显示
2 按钮淡入
3 按钮淡出

4 按钮被指上
5 按钮不再被指上
6 按钮被按下
7 按钮被松开
*/

/* m_showstat 状态
0 普通按钮状态
1 按钮被指上状态
2 按钮被按下状态
*/

bool GameMenuBtn::OnEvent(const SDL_Event& e, ::Snow::Activity& a)
{
    if(e.type == SDL_MOUSEBUTTONDOWN)
        if(InRect(m_rect,e.button.x,e.button.y))
            m_showstat = 2;

    if(e.type == SDL_MOUSEBUTTONUP)
        if(InRect(m_rect,e.button.x,e.button.y))
        {
            m_showstat = 1;
            ActivityEvent(0);
            return true;
        }
    if(e.type == SDL_MOUSEMOTION){
        if(InRect(m_rect,e.button.x,e.button.y))
        {
            m_showstat = 1;
            ActivityEvent(1);
            return true;
        }
        else m_showstat = 0;
    }
    return false;   //todo
}

void GameMenuBtn::SetPos(int x, int y)
{
    m_x = x;m_y = y;
    m_rect.x = x;m_rect.y = y;
    m_btn.GetSize(m_rect.w,m_rect.h);
}

void GameMenuBtn::Show()
{
    m_stat = 2;
    m_visible_l = true;
    m_timer.Reset();
}

void GameMenuBtn::Hide()
{
    m_stat = 3;
    m_visible_l = false;
    m_timer.Reset();
}

void GameMenuBtn::Destory()
{
    m_btn.Clear();
}

void GameMenuBtn::Init(const string& menuBtn)
{
    m_btn.Load(menuBtn);
    m_visible_l = false;
    m_stat = 0; //不显示
}

void GameMenuBtn::OnDraw()
{
    if(m_stat != 0){
        m_btn.OnDraw();
    }
}

void GameMenuBtn::OnNext()
{
    if(m_stat == 1)switch(m_showstat){
    case 0:
        m_btn.SetAlpha(192);
        break;
    case 1:
        m_btn.SetAlpha(255);
        break;
    case 2:
        m_btn.SetAlpha(128);
        break;
    }

    if(m_stat == 2)
    {
        float x = float(m_timer.GetTimer()) / 128;
        if(x>=1.0){m_stat = 1;m_btn.SetAlpha(192);m_btn.SetPos(m_x,m_y);}
        m_btn.SetAlpha(192 * x);
        m_btn.SetPos(int(m_x + (1 - x) * (pRnd.GetW() - m_x)),m_y);
    }else if(m_stat == 3){
        float x = float(m_timer.GetTimer()) / 128;
        if(x>=1.0) m_stat = 0;
        m_btn.SetAlpha(192 * (1 - x));
        m_btn.SetPos(int(m_x + x * (pRnd.GetW() - m_x)),m_y);
    }
}




