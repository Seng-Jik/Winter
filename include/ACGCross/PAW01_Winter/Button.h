#pragma once
#include "Snow.h"

namespace ACGCross{
class Button : public ::Snow::Control
{
    public:

        Button();
        void SetNormalPic(const std::string& normalPic);
        void SetMotionPic(const std::string&);
        void SetDownPic(const std::string&);
        void SetAlpha(Uint8 alp);
        void SetPos(int x,int y);

        inline ::Snow::Sprite* GetNormal(){return &m_normal;}
        inline ::Snow::Sprite* GetMotion(){return &m_motion;}
        inline ::Snow::Sprite* GetDown(){return &m_down;}

        inline void SetEnable(bool b){m_enable = b;m_stat = 1;};
        void OnDraw();
        bool OnEvent (const SDL_Event&,::Snow::Activity&);

        void Show();
        void Hide();
        bool GetHidden(){return m_stat == 0;}
    protected:
        int m_stat;
        bool m_enable = true;
        SDL_Rect m_r;
        //0-Hidden
        //1-Normal
        //2-Motion
        //3-MouseDown

        ::Snow::Sprite m_normal;
        ::Snow::Sprite m_motion;
        ::Snow::Sprite m_down;
    private:

};
}
