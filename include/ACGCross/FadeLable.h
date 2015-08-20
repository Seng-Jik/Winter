#ifndef _HEAD_FADELABLE_HEAD_
#define _HEAD_FADELABLE_HEAD_
#include "Core.h"

namespace ACGCross{

class FadeLable
{
    public:
        FadeLable();
        void OnDraw();
        void OnNext();

        void Hide();
        void Show();

        void SetFadeTime(int ms);
        void SetText(const std::wstring&);
        void SetAlpha(Uint8 alp);
        void SetColor(Uint8 r,Uint8 g,Uint8 b);
        void SetSize(int hei);
        void SetPos(int,int);
        void SetFont(Core::Font* f);
        void UpdateText();
        void UpdateTextFast();

        std::wstring& GetText();
    protected:
        enum STATE{
            NOTHING,
            HIDING,
            SHOWING,
            HIDING_CHG
        }m_stat;
        std::wstring m_text;
    private:
        Uint8 m_color[3];
        Uint8 m_size;
        int m_pos[2];
        int m_fpsTime;
        Core::FPSTimer m_timer;

        Core::Texture m_tex;
        Core::Font* m_fnt;
};
}

#endif // _HEAD_FADELABLE_HEAD_
