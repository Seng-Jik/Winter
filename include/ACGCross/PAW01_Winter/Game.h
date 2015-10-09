#pragma once

#include "GameDataMgr.h"
#include "Snow.h"
#include "MousePointer.h"

namespace ACGCross{
    namespace Galgame{
        class TextBox;
        class Clock;
    }
    extern GameDataMgr gameData;
    extern ::Snow::ResVal r;

    extern ::Snow::Activity* pGal;
    extern ::Snow::Activity* pTitle;
    extern ::Snow::Activity* pEd;
    extern ::Snow::Activity* pSaveUI;
    extern ::Snow::Activity* pTitle;
    extern ::Snow::Activity* pSettingUI;
    extern ::Snow::Activity* pCGViewer;
    extern Galgame::TextBox* pTextBox;
    extern Galgame::Clock* pClock;
    extern MousePointer* pMouse;


    void AutoSaver(::Snow::THREAD_ID);
    void InitGame();
    void QuitGame();

}
