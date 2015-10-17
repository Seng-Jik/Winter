#pragma once
#include "Snow.h"
#include <list>
namespace ACGCross{
class MousePointer
{
    public:
        MousePointer();
        void Load(const std::string&);
        void OnDraw();
        void OnEvent(const SDL_Event& e);
        void NewPoi(int x,int y);
        ~MousePointer();
    protected:
    private:
        Snow::Sprite m_mouse;
        struct MouseEffPoi{
            int x,y;
            int cnt;
            bool live;
        };
        MouseEffPoi m_pois [8];
        SDL_Texture* m_mpoiTex;
};
}
