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
}

#include <iostream>
void MousePointer::OnDraw()
{
    if(SDL_GetMouseFocus()) m_mouse.OnDraw();
}

void MousePointer::OnEvent(const SDL_Event& e)
{
    if(e.type == SDL_MOUSEMOTION) m_mouse.SetPos(e.motion.x,e.motion.y);
}
