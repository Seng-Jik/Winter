#include "ACGCross/PAW01_Winter/NumTexture.h"
#include "Snow/Debug.h"
using namespace ACGCross;



void NumSprite::SetNum(char n)
{
    int w,h;
    GetSize(w,h);
    w /= 10;
    SetSrc(w * (n - '0'),0,w,h);
}


void NumSprite::Load(const std::string& fileName)
{
    PNT("NUMT");
    Sprite::Load(fileName);
    PNT("NUMT2");
    int w,h;
    GetSize(w,h);
    PNT("NUMT3");
    SetZoom(w/10,h);
    PNT("NUMT4");
}

int NumSprite::GetWei()
{
    int w,h;
    GetSize(w,h);
    return w / 10;
}


