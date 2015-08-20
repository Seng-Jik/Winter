#ifndef HEAD_ACGCROSS_GALGAME_GAMELOGO_
#define HEAD_ACGCROSS_GALGAME_GAMELOGO_

#include "ACGCross/GalgameActivity/Corner.h"

namespace ACGCross{
namespace Galgame{


class GameLogoBg : public ACGCross::Galgame::Corner
{
    public:
        void Init();
        void OnNext();
        inline float GetPer(){return float(m_fpsCounter)/m_fadeFpsTime;};
    protected:
    private:
};

}
}

#endif // HEAD_ACGCROSS_GALGAME_GAMELOGO_
