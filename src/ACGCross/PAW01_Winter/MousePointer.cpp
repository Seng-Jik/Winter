#include "ACGCross\PAW01_Winter\MousePointer.h"
using namespace ACGCross;
using namespace Snow;

MousePointer::MousePointer()
{
    SDL_ShowCursor(0);
}

MousePointer::~MousePointer()
{
    //dtor
}

void MousePointer::Load(const std::string& _)
{
    m_mouse.Load(_);
    m_mouse.SetAlpha(112);
    Surface sur;
    sur.Load("GalGameSystem/mouse_out.png");
    m_mpoiTex = SDL_CreateTextureFromSurface(pRnd,sur);
}

#include "MathFunc.h"
#include "Snow/Debug.h"
void MousePointer::OnDraw()
{
    try{
    REDRAW:
    FOR_EACH(p,m_pois.begin(),m_pois.end()){
        if(p->cnt>0){
            float per = ACGCross::ArcFunc(float(p->cnt)/20);
            if(per == -1){
                m_pois.erase(p);
                goto REDRAW;
            }
            else{
                SDL_Rect r;
                r.x = p ->x - int(32 * per);
                r.y = p ->y - int(32 * per);
                r.w = int(64*per);r.h=int(64*per);
                SDL_SetTextureAlphaMod(m_mpoiTex,255*(1-per));
                SDL_RenderCopy(pRnd,m_mpoiTex,nullptr,&r);
            }
        }
        ++p->cnt;
    }
    }catch(...){
        std::cout<<"Fuck::"<<m_pois.size()<<std::endl;
        system("pause");
    }
    if(SDL_GetMouseFocus()) m_mouse.OnDraw();
}

void MousePointer::OnEvent(const SDL_Event& e)
{
    if(e.type == SDL_MOUSEMOTION) m_mouse.SetPos(e.motion.x,e.motion.y);
    else if(e.type == SDL_MOUSEBUTTONDOWN){
        m_pois.push_back({e.button.x,e.button.y,0});
    }else if(e.type == SDL_MOUSEBUTTONUP && e.button.button == 1){
        //m_pois.push_back({e.button.x,e.button.y,0});
        m_pois.push_back({e.button.x,e.button.y,0});
        //m_pois.push_back({e.button.x,e.button.y,-20});
    }
}
