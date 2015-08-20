#pragma once
#include "Core.h"
#include "ACGCross/Button.h"

namespace ACGCross{
class TitleButton : public Button
{
    public:
        void OnNext();
        void Show();
        void Hide();
    protected:
        float m_per;
        Core::FPSTimer m_fpsCounter;
};
}
