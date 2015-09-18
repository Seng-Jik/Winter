#include "ACGCross/PAW01_Winter/Title/GameLogo.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"

#define W 96
#define H 110

using namespace ACGCross::Galgame;

void GameLogoBg::Init()
{
    Corner::Init();
    m_corner.SetRollCenter(W,H);
}

void GameLogoBg::OnNext()
{
    Corner::OnNext();
    m_ang += 0.20;

    if(m_stat == -2){
        float per = ArcFunc(float(m_fpsCounter) / m_fadeFpsTime);
        m_corner.SetPos(int(20+(W-W*per)),int(10+(H-H*per)));
        m_corner.SetZoom(2*W*per,2*H*per);
    }
    else if(m_stat == 1)
    {
        m_corner.SetPos(20,10);
        m_corner.SetZoom(W*2,H*2);
    }
}
