#pragma once
#include "Snow.h"
#include "ACGCross/PAW01_Winter/Button.h"

namespace ACGCross{
class TitleButton : public Button
{
    public:
        void OnNext();
        void Show();
        void Hide();
    protected:
        float m_per;
        ::Snow::FrameTimer m_fpsCounter;
};
}
