#ifndef _HEAD_ACGCROSS_GALGAME_CORNER_
#define _HEAD_ACGCROSS_GALGAME_CORNER_

#include "Core.h"

namespace ACGCross {
namespace Galgame {


class Corner
{
    public:
        void Init();
        void Destory();
        void OnNext();
        void OnDraw();

        void Hide();
        void Show();
        bool Finished();

        bool GetVisible();

        void SetFadeSpeed(int time);
        void SetTopAlpha(int a);
        void SetSrc(int x,int y,int w,int h){
            m_corner.SetSrc(x,y,w,h);
        }
        void SetPos(int x,int y){
            m_corner.SetPos(x,y);
        }
        void SetZoom(int w,int h){
            m_corner.SetZoom(w,h);
        }
        void SetRollCenter(int x,int y){
            m_corner.SetRollCenter(x,y);
        }

    protected:
        Core::Texture m_corner;
        int m_fadeFpsTime;
        Core::FPSTimer m_fpsCounter;
        int m_stat;
        int m_topAlpha;
        double m_ang;
    private:


};

}
}

#endif // _HEAD_ACGCROSS_GALGAME_CORNER_
