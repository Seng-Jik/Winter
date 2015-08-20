#ifndef _HEAD_SLIDER_HEAD_
#define _HEAD_SLIDER_HEAD_

#include "Core.h"
#include <string>

namespace ACGCross{

class Slider : public Core::Control
{
    public:
        bool OnEvent (const SDL_Event&,Core::Activity& a);
        // 1 - 移动滑块了
        // 2 - 被按下了
        // 3 - 从这里抬起了

        inline float GetValue(){return float(m_hkxpos_xd)/m_r.w;};
        void SetValue(float);
        void SetPos(int x,int y);
        void SetLength(int w);
        void SetAlpha(Uint8 alp);   //bug:只是设置了全局的Alpha值，并没有兼容其它因素造成的影响

        void SetBKTex(const std::string&);
        void SetHKTex(const std::string&);
        void SetHKTex_Mot(const std::string&);
        void SetHKTex_Down(const std::string&);
        void OnDraw();
        //void OnNext();
    protected:
    private:
        enum MouseState{
            DOW,MOT,NOR
        }m_mouse=NOR;
        Core::Texture m_lener;
        Core::Texture m_hk;
        Core::Texture m_hkm;
        Core::Texture m_hkd;
        SDL_Rect m_r = {0,0,0,0};
        SDL_Rect m_re = {0,0,0,0};
        int m_hkxpos_xd = 0;
        int m_hkw = 0;
        int m_hkh = 0;
        int m_mouse_offset = 0;
};
}

#endif // _HEAD_SLIDER_HEAD_
