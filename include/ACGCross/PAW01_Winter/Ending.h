#ifndef _HEAD_ENDING_HEAD_
#define _HEAD_ENDING_HEAD_

#include "Snow.h"

namespace ACGCross{

class Ending : public ::Snow::Activity
{
    public:
        Ending();
        void OnShow();
        void OnHide();
        void OnDraw();
        void OnNext();
    protected:
    private:
        ::Snow::Sprite m_ed;
        ::Snow::Timer m_timer;
        ::Snow::Sound m_bgm;
        int m_edtime;
        int m_staffHeightCount;
        int m_edx;
};
}

#endif // _HEAD_ENDING_HEAD_
