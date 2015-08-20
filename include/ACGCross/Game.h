#pragma once

#include "GameDataMgr.h"
#include "Core.h"

namespace ACGCross{
    namespace Galgame{
        class TextBox;
        class Clock;
    }
    extern GameDataMgr gameData;
    extern Core::ResVal r;

    extern Core::Activity* pGal;
    extern Core::Activity* pTitle;
    extern Core::Activity* pEd;
    extern Core::Activity* pSaveUI;
    extern Core::Activity* pTitle;
    extern Core::Activity* pSettingUI;
    extern Galgame::TextBox* pTextBox;
    extern Galgame::Clock* pClock;


    void AutoSaver(Core::THREAD_ID);
    void InitGame();
    void QuitGame();

}
