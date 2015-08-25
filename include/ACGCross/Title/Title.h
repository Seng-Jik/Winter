#pragma once

#include "Core.h"
#include "ACGCross/MathFunc.h"
#include "ACGCross/Title/MovingBg.h"
#include "ACGCross/GalgameActivity/Snow.h"
#include "ACGCross/Title/GameLogo.h"
#include "ACGCross/Title/TitleButton.h"
#include <vector>

namespace ACGCross{
namespace Galgame{

    class Title:public Core::Activity
    {
    private:
        MovingBg m_mbg;
        Snow m_snow;
        GameLogoBg m_lbg;
        Core::Texture m_acgcrossLogo;
        Core::Texture m_fk;
        int m_stat;
        //0-普通
        //1-正在显示
        //2-正在显示按钮
        //3-正在隐藏
        //4-正在隐藏按钮

        int m_showedBtn_temp;

        TitleButton m_bSta;
        TitleButton m_bContinue;
        TitleButton m_bLoad;
        TitleButton m_bCG;
        TitleButton m_bExit;
        int m_dataNum = -1;
        std::vector<TitleButton*> m_visableBtn;

        TitleButton* m_btnOpr;
        bool m_dosomething = false;

        Core::FPSTimer m_fpsCounter;

    public:
        Title();
        inline void SetLoad(int n){m_dataNum = n;}
        void OnInit();
        void OnShow();
        void OnHide();
        void HideButton();
        void ShowButton();
        void FadeToBlackAndOpr();
        void OnNext();
        void OnDraw();
        void OnEvent(const SDL_Event&);
        void OnEvent(Core::Control*, const Sint32);
    };
}
}
