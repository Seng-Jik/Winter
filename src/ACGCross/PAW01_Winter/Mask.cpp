#include "ACGCross/PAW01_Winter/Mask.h"

using namespace std;
using namespace Core;
using namespace ACGCross;

void Mask::LoadMaskPic(const std::string& s)
{
    Surface sur;
    sur.Load(s);
    sur.ConvertFormat(SDL_PIXELFORMAT_ARGB8888);

    m_w = ((SDL_Surface*)sur) ->w;
    m_h = ((SDL_Surface*)sur) ->h;
    Uint32* pPx = sur.GetPixels();
    m_mask = new Uint8 [m_w * m_h];

    sur.Lock();
    for(Uint32 off = 0;off < Uint32(m_w * m_h);++off){
        m_mask[off] = pPx[off] & 0x000000FF;
    }
    sur.Unlock();
}

void Mask::Clear()
{
    m_w = m_h = 0;
    if(m_mask != nullptr) delete [] m_mask;
    m_mask = nullptr;
}

void Mask::ApplyToSurfaceARGB8888(SDL_Surface* pSur, float progress)
{
    SDL_LockSurface(pSur);
    if(progress >= 1){
        for(Uint32 offset = 0;offset < Uint32(pSur ->h * pSur ->w);++offset){
            *(((Uint8*)pSur -> pixels) +4*offset +3) =  255;
        }
    }
    else{
        for(int y = 0;y < pSur ->h;++y){
            int y_inMask = (float(y) / pSur ->h) * m_h;
            if(y_inMask >= m_h) y_inMask = m_h -1;

            for(int x = 0;x < pSur ->w;++x){
                int x_inMask  = (float(x) / pSur ->w)* m_h;
                if(x_inMask >= m_w) x_inMask = m_h -1;

                register Sint16 alp =  510 * progress - m_mask[y_inMask * m_w + x_inMask];
                if(alp > 255) alp = 255;
                else if(alp < 0) alp = 0;
                *(((Uint8*)pSur -> pixels) + y * pSur ->pitch + 4*x +3) =  alp;
            }
        }
    }
    SDL_UnlockSurface(pSur);
}

