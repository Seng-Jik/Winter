#include "ACGCross\NumTexture.h"
using namespace ACGCross;

#include <fstream>
std::fstream logloglo("CLOCK_log.log",std::ios::out);
#define PNT(s) logloglo<<s<<std::endl;


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


