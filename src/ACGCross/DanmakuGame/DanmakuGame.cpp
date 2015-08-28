#include "ACGCross/DanmakuGame/DanmakuGame.h"
#include "Core.h"
#include <iostream>
using namespace std;
using namespace Core;

DanmakuGame::DanmakuGame()
{
    //ctor
}

DanmakuGame::~DanmakuGame()
{
    //dtor
}

void DanmakuGame::OnNext()
{
    int spd = 5;
    if(m_sloD) spd = 2;
    if(m_doD && m_char_pos.y < 768) m_char_pos.y+=spd;
    if(m_upD && m_char_pos.y > 0) m_char_pos.y-=spd;
    if(m_leD && m_char_pos.x < 1024) m_char_pos.x-=spd;
    if(m_riD && m_char_pos.x > 0) m_char_pos.x+=spd;
}

void DanmakuGame::OnDraw()
{
    SDL_RenderCopy(pRnd,m_char,&m_char_k,&m_char_pos);
}

void DanmakuGame::OnHide()
{
    SDL_DestroyTexture(m_char);
}

void DanmakuGame::OnShow()
{
    m_char = IMG_LoadTexture(pRnd,"danmukuchar.png");
    SDL_SetTextureBlendMode(m_char,SDL_BLENDMODE_BLEND);
    SDL_QueryTexture(m_char,nullptr,nullptr,&m_char_k.w,&m_char_k.h);
    m_char_k.x = m_char_k.y = 0;
    m_char_pos = m_char_k;
    m_char_pos.x += 530;
    m_char_pos.y += 530;

    m_sloD=m_upD=m_riD=m_leD=m_doD = false;
}

void DanmakuGame::OnKeyEvent(KeyEvent s, Uint16 e)
{
    cout<<s<<","<<e<<endl;
    if(s == KEYUP) switch(e){
    case GKEY_DOWN:
        m_doD = false;break;
    case GKEY_UP:
        m_upD = false;break;
    case GKEY_LEFT:
        m_leD = false;break;
    case GKEY_RIGHT:
        m_riD = false;break;
    case GKEY_SLOW:
        m_sloD = false;break;
    }

    if(s == KEYDOWN) switch(e){
    case GKEY_DOWN:
        m_doD = true;break;
    case GKEY_UP:
        m_upD = true;break;
    case GKEY_LEFT:
        m_leD = true;break;
    case GKEY_RIGHT:
        m_riD = true;break;
    case GKEY_SLOW:
        m_sloD = true;break;
    }
}

void DanmakuGame::OnEvent(const SDL_Event& e)
{
    KeyMapActivity::OnEvent(e);
}
