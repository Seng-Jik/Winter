#pragma once
#include "Snow.h"
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
            int x,y,cnt;
        };
        SDL_Texture* m_mpoiTex;
};
}
