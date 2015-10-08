#pragma once
#include "../Snow/Sprite.h"
namespace ACGCross{
class NumSprite:public ::Snow::Sprite
{
public:
    void SetNum(char n);
    void Load(const std::string&);
    int GetWei();
};
}
