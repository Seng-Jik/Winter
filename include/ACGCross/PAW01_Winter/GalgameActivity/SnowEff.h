#pragma once
#include "Snow.h"
#include "Snow/SDLbase.h"
#include "SMEProcUnit.h"
#include "SMLanguage/SMI.h"
#include <list>

namespace ACGCross{
namespace Galgame{

class SnowEff:public SMEProcUnit
{
    struct SnowStat{
        SnowStat(){};
        SDL_Rect rect;
        float fpsSpeed; //一帧垂直移动像素数
        float fpsCounter; //超过1时移动其整数值后求余1
        float windSpeed; //fps风速
        float windCounter;
    };
    enum Stat{SNOWING,STOPPING,STOPPED};
private:
    SDL_Texture* m_tex;
    Snow::ResFile m_file;
    std::list<SnowStat> m_statList;
    int m_wind; //1s风速
    int m_wind_p;   //1s风速偏差值
    int m_speed; //1s速度
    int m_speed_p; //速度偏差
    Uint16 m_volume; //降雪量
    Stat m_stat;
    void InitSnowStat(SnowStat* s,bool init = false);
    void GetNextFpsStat();
public:
    SnowEff();
    void Init();
    void Quit();
    void OnDraw();
    void OnNext();
    void SetSpeed(int s, int p);
    void SetWind(int w, int p);
    void SetVolume(Uint16 v);
    void Stop();
    void ForceStop();
    void SaveState(Snow::Bundle<65536>&);
    void LoadState(Snow::Bundle<65536>&);
    void Start();
    inline Stat Status(){return m_stat;};

    virtual void SMEProc(SMI::SMEvent*);
    virtual void SMEFastProc(SMI::SMEvent*);
};

}
}

