#include "MaskCmdTarget.h"

using namespace Core;
using namespace ACGCross;
using namespace ACGCross::Galgame;

MaskCmdTarget::MaskCmdTarget(Bg* b)
{
    m_bg = b;
    m_white = SDL_CreateRGBSurface(0,pRnd.GetPhW(),pRnd.GetPhH(),32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    Uint32* pPx = (Uint32*)m_white -> pixels;
    for(Sint32 i = 0;i < m_white -> w * m_white -> h;++i)
        pPx[i] = 0x00FFFFFF;

    m_black = SDL_CreateRGBSurface(0,pRnd.GetPhW(),pRnd.GetPhH(),32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    pPx = (Uint32*)m_black -> pixels;
    for(Sint32 i = 0;i < m_white -> w * m_white -> h;++i)
        *(pPx+i) = 0x00FFFFFF;
}

MaskCmdTarget::~MaskCmdTarget(){
    SDL_FreeSurface(m_black);
    SDL_FreeSurface(m_white);
}

void MaskCmdTarget::SMEFastProc(SMI::SMEvent*)
{
    StopMask();
}

bool MaskCmdTarget::SMEFinished(SMI::SMEvent*)
{
    return m_stat == NOTHING;
}

void MaskCmdTarget::SMEProc(SMI::SMEvent* pSme)
{
    //It must "bg_mask" cmd.
    m_m.LoadMaskPic("GalRes/Masks/"+WStringToString(pSme ->argv[0])+".png");
    if(pSme -> argv.size() == 7) DoMask("GalRes/bg/"+WStringToString(pSme ->argv[1])+".png",
           _wtoi(pSme->argv[2].c_str()),
           _wtoi(pSme ->argv[3].c_str()),
            _wtoi(pSme->argv[4].c_str()),
           float(_wtof(pSme->argv[5].c_str())),
           _wtoi(pSme->argv[6].c_str()));
    else if(pSme -> argv.size() == 3) DoMask("GalRes/bg/"+WStringToString(pSme ->argv[1])+".png",
           0,0,255,1.0f,_wtoi(pSme->argv[2].c_str()));
    else if(pSme -> argv.size() == 2) DoMask("GalRes/bg/"+WStringToString(pSme ->argv[1])+".png",
           0,0,255,1.0f,1000);
}

void MaskCmdTarget::LoadMask(const std::string& s)
{
    m_m.LoadMaskPic(s);
}

void MaskCmdTarget::DoMask(const std::string& s, int cen_x, int cen_y, int alp, float zoom, int time)
{
    m_arg_file = s;m_arg_cenx = cen_x;m_arg_ceny = cen_y;m_arg_alp = alp;m_arg_zoom = zoom;

    m_stat = MASKING_FR;m_time = time;
    m_newBg.Load(s,cen_x,cen_y,zoom,alp);

    SDL_Rect scr;
    m_newBg.GetPos(scr.x,scr.y,scr.w,scr.h);
    scr.h -= scr.y;scr.w-=scr.x;

    m_newBgSur = SDL_CreateRGBSurface(0,pRnd.GetPhW(),pRnd.GetPhH(),32,0x00FF0000,0x0000FF00,0x000000FF,0xFF000000);
    SDL_BlitSurface(m_newBg.GetSurface(),nullptr,m_newBgSur,&scr);
    SDL_SetSurfaceBlendMode(m_newBgSur,SDL_BLENDMODE_BLEND);
    SDL_SetSurfaceRLE(m_newBgSur,1);

    m_tex.Clear();
    m_tex.SetPos(0,0);
    m_tex.SetZoom(pRnd.GetW(),pRnd.GetH());
    m_timer.Reset();
}

void MaskCmdTarget::OnDraw()
{
    if(m_stat != NOTHING) m_tex.OnDraw();
}

void MaskCmdTarget::OnNext()
{
    if(m_stat == NOTHING) return;
    else{
        float per = float(m_timer.GetTimer()) / m_time;
        if(per >= 1.0) StopMask();
        else{
            m_m.ApplyToSurfaceARGB8888(m_newBgSur,per);
            m_tex.Load(m_newBgSur);
        }
    }
}

void MaskCmdTarget::StopMask()
{
    SDL_FreeSurface(m_newBgSur);m_newBgSur = nullptr;
    m_stat = NOTHING;
    m_bg -> Load(m_arg_file,m_arg_cenx,m_arg_ceny,m_arg_zoom,m_arg_alp);
    m_bg -> OnNext();
    m_tex.Clear();
}

