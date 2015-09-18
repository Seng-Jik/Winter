#ifndef _HEAD_ENDING_HEAD_
#define _HEAD_ENDING_HEAD_

#include "Core.h"

namespace ACGCross{

class Ending : public Core::Activity
{
    public:
        Ending();
        void OnShow();
        void OnHide();
        void OnDraw();
        void OnNext();
    protected:
    private:
        Core::Texture m_ed;
        Core::Timer m_timer;
        Core::Sound m_bgm;
        int m_edtime;
        int m_staffHeightCount;
        int m_edx;
};
}

#endif // _HEAD_ENDING_HEAD_
