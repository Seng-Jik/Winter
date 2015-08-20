#pragma once
#include "Core.h"

namespace ACGCross{
class Button : public Core::Control
{
    public:

        Button();
        void SetNormalPic(const std::string& normalPic);
        void SetMotionPic(const std::string&);
        void SetDownPic(const std::string&);
        void SetAlpha(Uint8 alp);
        void SetPos(int x,int y);

        inline Core::Texture* GetNormal(){return &m_normal;}
        inline Core::Texture* GetMotion(){return &m_motion;}
        inline Core::Texture* GetDown(){return &m_down;}

        void OnDraw();
        bool OnEvent (const SDL_Event&,Core::Activity&);

        void Show();
        void Hide();
    protected:
        int m_stat;
        SDL_Rect m_r;
        //0-Hidden
        //1-Normal
        //2-Motion
        //3-MouseDown

        Core::Texture m_normal;
        Core::Texture m_motion;
        Core::Texture m_down;
    private:

};
}
