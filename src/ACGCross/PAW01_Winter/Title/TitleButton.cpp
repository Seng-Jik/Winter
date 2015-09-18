#include "ACGCross/PAW01_Winter/Title/TitleButton.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"
#define FPSSPEED 30
#define YOFFEST 90
using namespace std;
using namespace Core;
using namespace ACGCross;

void TitleButton::Hide()
{
    if(m_stat == -1) return;
    m_stat = -1;
    m_fpsCounter.Reset();
}

void TitleButton::Show()
{
    if(m_stat == -2) return;
    m_stat = -2;
    m_fpsCounter.Reset();
}

void TitleButton::OnNext()
{
    m_per = float(m_fpsCounter) / FPSSPEED;

    if(m_stat == -2){
        if(m_per >= 1){
            m_stat = 1;
            m_normal.SetPos(m_r.x,m_r.y);
            m_motion.SetPos(m_r.x,m_r.y);
            m_down.SetPos(m_r.x,m_r.y);
            SetAlpha(255);
            return;
        }
        m_normal.SetPos(m_r.x,int(m_r.y + YOFFEST*FArcFunc(1-m_per)));
        m_motion.SetPos(m_r.x,int(m_r.y + YOFFEST*FArcFunc(1-m_per)));
        m_down.SetPos(m_r.x,int(m_r.y + YOFFEST*FArcFunc(1-m_per)));
        SetAlpha(255*ArcFunc(FArcFunc(m_per)));
    }
    else if(m_stat == -1){
        if(m_per >= 1){
            m_stat = 0;
            return;
        }
        m_normal.SetPos(m_r.x,int(m_r.y + YOFFEST*FArcFunc(m_per)));
        m_motion.SetPos(m_r.x,int(m_r.y + YOFFEST*FArcFunc(m_per)));
        m_down.SetPos(m_r.x,int(m_r.y + YOFFEST*FArcFunc(m_per)));
        SetAlpha(255*ArcFunc(1-m_per));
    }
}

