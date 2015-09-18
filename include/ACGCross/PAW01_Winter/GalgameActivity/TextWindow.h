#ifndef _HEAD_TEXTWINDOW_HEAD_
#define _HEAD_TEXTWINDOW_HEAD_
#include "Core.h"
#include <list>

namespace ACGCross{
namespace Galgame{

class TextWindow
{
public:
    class Widget;
private:
    std::list<Widget*> m_widl;
    int m_fadeFpsTime;
    int m_posy;
    Core::Texture m_tex;
    Core::FPSTimer m_fps;
    enum STATE{
        NOTHING,
        SHOWING,
        HIDING_OT,
        HIDING,
        HIDDEN
    } m_stat = NOTHING;
public:
    class Widget{
    friend class TextWindow;
    private:
        int m_TWx,m_TWy;

    public:
        virtual void TWPos(int x,int y) = 0;
        virtual void TWAlpha(Uint8) = 0;
    };
    TextWindow();
    void OnNext();
    void OnDraw();
    inline bool Finished(){return m_stat == NOTHING || m_stat == HIDDEN;};

    inline void Show(){m_fps.Reset();m_stat = SHOWING;};
    void Hide();
    inline bool GetVisible(){return m_stat != HIDDEN;};

    void RegWig(Widget& w,int x,int y);

    void Init();
    void Quit();
protected:

};
}
}
#endif // _HEAD_TEXTWINDOW_HEAD_
