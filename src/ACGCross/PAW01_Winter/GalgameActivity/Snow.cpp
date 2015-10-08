#include "ACGCross/PAW01_Winter/GalgameActivity/SnowEff.h"
#include "stdlib.h"
using namespace ::Snow;
using namespace ACGCross::Galgame;

void SnowEff::InitSnowStat(SnowEff::SnowStat* s,bool init)    //init为true时自动分配rect的h
{
    SDL_QueryTexture(m_tex,nullptr,nullptr,&(s->rect.w),&(s->rect.h));
    s ->rect.h = s -> rect.w = rand() % s->rect.w;


    if(init) s -> rect.y = -(rand()%pRnd.GetH());
    else s -> rect.y = -(s ->rect.h);

    //完成rect

    /*
    Sint32 temp;
    if(rand()%2 == 0)
        temp = rand()%m_speed_p;
    else
        temp = -(rand()%m_speed_p);
    temp += m_speed;*/
    int t;
    if(bool(rand()%2))
        t = m_speed - (rand()%m_speed_p);
    else
        t = m_speed + (rand()%m_speed_p);

    s -> fpsSpeed = float(t)/FPS;
    s -> fpsCounter = 0;

    /*
    temp = rand()%2;
    if(temp)
        temp = rand()%m_wind_p;
    else
        temp = -(rand()%m_wind_p);
    temp += m_wind;*/
    if(bool(rand()%2))
        t = m_wind - (rand()%m_wind_p);
    else
        t = m_wind + (rand()%m_wind_p);
    s -> windSpeed = float(t)/FPS;
    s -> windCounter = 0;

    s -> rect.x = rand()%pRnd.GetW() - (s -> windSpeed)*(pRnd.GetH()/float(s -> fpsSpeed)/2);
}

SnowEff::SnowEff(){
    m_tex = nullptr;
    m_wind = 0;
    m_wind_p = 1;
    m_speed = 128;
    m_speed_p = 1;
    m_volume = 16;
    m_stat = STOPPED;
}

void SnowEff::Init(){
    m_file.Load("GalGameSystem/Snow.png");
    m_tex = IMG_LoadTexture_RW(pRnd,m_file,m_file.Size());
    //m_file.Free();
    SDL_SetTextureBlendMode(m_tex,SDL_BLENDMODE_BLEND);
}

void SnowEff::Quit(){
    m_stat = STOPPED;
    m_statList.clear();
    SDL_DestroyTexture(m_tex);
    m_tex = nullptr;
    m_file.Free();
}

void SnowEff::OnDraw(){
    if(m_stat == STOPPED) return;
    FOR_EACH(p,m_statList.begin(),m_statList.end())
    {
        SDL_RenderCopy(pRnd,m_tex,nullptr,&(p -> rect));
    }

}

void SnowEff::Start()
{
    m_stat = SNOWING;
    m_statList.resize(m_volume);
    FOR_EACH(p,m_statList.begin(),m_statList.end())
    InitSnowStat(&(*p),true);
}

void SnowEff::Stop()
{
    m_stat = STOPPING;
}

void SnowEff::SetVolume(Uint16 v)
{
    m_volume = v;
}

void SnowEff::SetWind(int w, int p)
{
    m_wind = w;
    m_wind_p = p;
}

void SnowEff::SetSpeed(int s, int p)
{
    m_speed = s;
    m_speed_p = p;
}

void SnowEff::OnNext()
{
    if(m_stat == STOPPED) return;

    if(m_stat == STOPPING){
        if(m_statList.empty()){
            m_stat = STOPPED;
            return;
        }
        FOR_EACH(p,m_statList.begin(),m_statList.end()){
            if(p->rect.y>=pRnd.GetH())
                m_statList.erase(p);
        }
        GetNextFpsStat();
    }

    if(m_stat == SNOWING){
        if(m_volume < m_statList.size())
        {
            FOR_EACH(p,m_statList.begin(),m_statList.end()){
                if(p->rect.y>=pRnd.GetH())
                    m_statList.erase(p);
            }
        }
        else if(m_volume > m_statList.size()){
            SnowStat s;
            InitSnowStat(&s);
            m_statList.push_back(s);
        }
        GetNextFpsStat();
    }
}

void SnowEff::GetNextFpsStat()
{
    FOR_EACH(p,m_statList.begin(),m_statList.end())
    {
        //速度
        p -> fpsCounter += p -> fpsSpeed;
        if(p -> fpsCounter >=1.0f){
            p -> rect.y += int(p -> fpsCounter);
            p -> fpsCounter -= int(p -> fpsCounter);
            if(p -> rect.y >= pRnd.GetH()) InitSnowStat(&(*p));
        }

        //风
        p -> windCounter += p -> windSpeed;
        if(p -> windCounter >=1.0f || p -> windCounter <= -1.0f){
            p -> rect.x += int(p -> windCounter);
            p -> windCounter -= int(p -> windCounter);
        }
    }
}

void SnowEff::SMEProc(SMI::SMEvent* e)
{
    if(e -> cmd == L"::Snow_start")
        Start();
    else if(e -> cmd == L"::Snow_setvol")
        SetVolume(_wtoi(e -> argv[0].c_str()));
    else if(e -> cmd == L"::Snow_setwind")
        SetWind(_wtoi(e -> argv[0].c_str()),_wtoi(e -> argv[1].c_str()));
    else if(e -> cmd == L"::Snow_setspeed")
        SetSpeed(_wtoi(e -> argv[0].c_str()),_wtoi(e -> argv[1].c_str()));
    else if(e -> cmd == L"::Snow_stop")
        Stop();
}

void SnowEff::SMEFastProc(SMI::SMEvent* e)
{

}

void SnowEff::ForceStop()
{
    m_stat = STOPPED;
    m_statList.clear();
}

void SnowEff::SaveState(Bundle<65536>& b)
{
    b.Write<Stat>(m_stat);
    b.Write<int>(m_wind);
    b.Write<int>(m_wind_p);
    b.Write<int>(m_speed);
    b.Write<int>(m_speed_p);
    b.Write<Uint16>(m_volume);
}

void SnowEff::LoadState(Bundle<65536>& b)
{
    b.Read<Stat>(m_stat);
    if(m_stat == STOPPED || m_stat == STOPPING) ForceStop();
    b.Read<int>(m_wind);
    b.Read<int>(m_wind_p);
    b.Read<int>(m_speed);
    b.Read<int>(m_speed_p);
    b.Read<Uint16>(m_volume);
}
