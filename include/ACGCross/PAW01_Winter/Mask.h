#pragma once
#include <string>
#include "Snow.h"

namespace ACGCross{

//这是个非常慢的类
class Mask
{
    public:
        void LoadMaskPic(const std::string&);
        void Clear();
        void ApplyToSurfaceARGB8888(SDL_Surface*,float progress);
        ~Mask(){Clear();}
    protected:
    private:
        Uint8* m_mask = nullptr;
        Uint16 m_w = 0,m_h= 0;
};

}
