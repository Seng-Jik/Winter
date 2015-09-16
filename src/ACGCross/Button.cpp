#include "Button.h"

using namespace std;
using namespace Core;
using namespace ACGCross;

void Button::Hide()
{
    m_stat = 0;
}

void Button::Show()
{
    m_stat = 1;
}


Button::Button()
{
    m_stat = 1;
}


bool Button::OnEvent(const SDL_Event& e, Activity& a)
{
    if(m_stat > 0){
        if(e.type == SDL_MOUSEMOTION){
            if(InRect(m_r,e.motion.x,e.motion.y)){
                m_stat = 2;
                ActivityEvent(2);
                return false;
            }else{
                m_stat = 1;
                return false;
            }
        }

        else if(e.type == SDL_MOUSEBUTTONDOWN){
            if(InRect(m_r,e.button.x,e.button.y)){
                m_stat = 3;
                ActivityEvent(3);
                return true;
            }
        }
        else if(e.type == SDL_MOUSEBUTTONUP){
            if(InRect(m_r,e.button.x,e.button.y)){
                m_stat = 2;
                ActivityEvent(4);
                return true;
            }
        }

        m_stat = 1;
    }
    return false;
}

void Button::OnDraw()
{
    /*if(m_stat >=2){
        int x,y;
        SDL_GetMouseState(&x,&y);
        if(!InRect(m_r,x,y)) m_stat = 1;
    }*/
    if(m_stat != 0) switch(m_stat){
    case 2:
        m_motion.OnDraw();break;
    case 3:
        m_down.OnDraw();break;
    default:
        m_normal.OnDraw();break;
    }
}

void Button::SetAlpha(Uint8 alp)
{
    m_normal.SetAlpha(alp);
    m_down.SetAlpha(alp);
    m_motion.SetAlpha(alp);

}

void Button::SetNormalPic(const string& str)
{
    m_normal.Load(str);
}

void Button::SetDownPic(const string& str)
{
    m_down.Load(str);
}

void Button::SetMotionPic(const string& s)
{
    m_motion.Load(s);
}

void Button::SetPos(int x,int y)
{
    m_r.x = x;
    m_r.y = y;
    int w,h;
    m_normal.GetSize(w,h);
    m_r.w = w;
    m_r.h = h;

    m_normal.SetPos(x,y);
    m_motion.SetPos(x,y);
    m_down.SetPos(x,y);
}
