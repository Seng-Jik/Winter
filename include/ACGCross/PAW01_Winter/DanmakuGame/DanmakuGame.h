#ifndef DANMAKUGAME_H
#define DANMAKUGAME_H

#include "ACGCross/PAW01_Winter/KeyMapActivity.h"

#define GKEY_UP 100
#define GKEY_DOWN 101
#define GKEY_LEFT 102
#define GKEY_RIGHT 103
#define GKEY_SLOW 110


class DanmakuGame : public ACGCross::KeyMapActivity
{
    public:
        DanmakuGame();

        void OnNext();
        void OnDraw();
        void OnHide();
        void OnShow();

        void OnEvent(const SDL_Event& e);
        void OnKeyEvent(KeyEvent,Uint16);

        virtual ~DanmakuGame();
    protected:
    private:
        SDL_Texture* m_char;
        SDL_Rect m_char_pos;
        SDL_Rect m_char_k;

        bool m_upD,m_riD,m_leD,m_doD,m_sloD;
};

#endif // DANMAKUGAME_H
