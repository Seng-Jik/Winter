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
        ~MousePointer();
    protected:
    private:
        Snow::Sprite m_mouse;
        struct MouseEffPoi{
            int x,y;
            int cnt;
        };
        std::list<MouseEffPoi> m_pois;
        SDL_Texture* m_mpoiTex;
};
}
