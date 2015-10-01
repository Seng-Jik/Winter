#include "ACGCross/PAW01_Winter/NumTexture.h"
#include "Core/Debug.h"
using namespace ACGCross;



void NumTexture::SetNum(char n)
{
    int w,h;
    GetSize(w,h);
    w /= 10;
    SetSrc(w * (n - '0'),0,w,h);
}


void NumTexture::Load(const std::string& fileName)
{
    PNT("NUMT");
    Texture::Load(fileName);
    PNT("NUMT2");
    int w,h;
    GetSize(w,h);
    PNT("NUMT3");
    SetZoom(w/10,h);
    PNT("NUMT4");
}

int NumTexture::GetWei()
{
    int w,h;
    GetSize(w,h);
    return w / 10;
}


