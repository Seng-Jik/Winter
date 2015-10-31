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

    FOR_EACH(i,0,8)
	    m_pois[i].live=false;

}

#include "MathFunc.h"
#include "Snow/Debug.h"
void MousePointer::OnDraw()
{
 	FOR_EACH(i,0,8){
		if(m_pois[i].live){
			if(m_pois[i].cnt>30){
				m_pois[i].live=false;
			}else{
				float per=ArcFunc(float(m_pois[i].cnt)/30);
				SDL_Rect r={
					m_pois[i].x-int(per*32),
					m_pois[i].y-int(per*32),
					int(64*per),int(64*per)
				};
				SDL_SetTextureAlphaMod(m_mpoiTex,255*(1-per));
				SDL_RenderCopy(pRnd,m_mpoiTex,nullptr,&r);
				++m_pois[i].cnt;
			}
		}
	}

    if(SDL_GetMouseFocus()) m_mouse.OnDraw();
}

void MousePointer::NewPoi(int x,int y){
	FOR_EACH(i,0,8){
		if(!m_pois[i].live){
			m_pois[i].x=x;
			m_pois[i].y=y;
			m_pois[i].cnt=0;
			m_pois[i].live=true;
			break;
		}
	}
}


void MousePointer::OnEvent(const SDL_Event& e)
{
    if(e.type == SDL_MOUSEMOTION) m_mouse.SetPos(e.motion.x,e.motion.y);
    else if(e.type == SDL_MOUSEBUTTONDOWN){
        NewPoi(e.button.x,e.button.y);
    }else if(e.type == SDL_MOUSEBUTTONUP && e.button.button == 1){
        //m_pois.push_back({e.button.x,e.button.y,0});
        NewPoi(e.button.x,e.button.y);
        //m_pois.push_back({e.button.x,e.button.y,-20});
    }
}
