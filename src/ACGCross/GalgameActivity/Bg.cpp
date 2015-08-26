#include "ACGCross/GalgameActivity/Bg.h"
#include "GalgameActivity.h"
#include "ACGCross/MathFunc.h"

using namespace Core;

namespace ACGCross{
namespace Galgame{

void Bg::Reset(){
    m_dp_mode = CENTER;
    m_WillAlpha = m_WillMove = m_WillZoom = false;
    IsLoaded = false;
    AfterLoad = false;
    m_alpha = 255;
    m_tex.Clear();
    m_sur.Clear();
}

Bg::Bg(){
    m_dp_mode = CENTER;
    IsLoaded = false;
    AfterLoad = false;
    m_alpha = 255;
}

Bg::~Bg(){
}

SDL_Surface* Bg::GetSurface(){
    return m_sur;
}

void Bg::Alpha(int p_a,int p_a_time){
    m_WillAlpha = true;
    m_dst_a = p_a;
    m_a_time = p_a_time;
    m_a_timer.Reset();
}

void Bg::Move(int p_dst_x,int p_dst_y,int p_m_time){
    m_dst_x = p_dst_x;
    m_dst_y = p_dst_y;
    m_WillMove = true;
    m_m_time = p_m_time;
    m_m_timer.Reset();
}

void Bg::Move_rt(int p_dst_x,int p_dst_y,int p_m_time){
    m_WillMove = true;
    m_dst_x = p_dst_x + m_rect.x;
    m_dst_y  = p_dst_y + m_rect.y;
    m_m_time = p_m_time;
    m_m_timer.Reset();
}

void Bg::Zoom(float p_z,int p_z_time){
    m_WillZoom = true;
    m_dst_w = m_src_w * p_z;
    m_dst_h = m_src_h * p_z;
    m_z_time = p_z_time;
    m_z_timer.Reset();
}

void Bg::Zoom(int p_dst_w,int p_dst_h,int p_z_time){
    m_WillZoom = true;
    m_dst_w = p_dst_w;
    m_dst_w = p_dst_h;
    m_z_time = p_z_time;
    m_z_timer.Reset();
}

void Bg::SetCenMove(int p_cen_x,int p_cen_y,float p_zoom,int p_time){
    Move( pRnd.GetW() / 2 -  p_cen_x,pRnd.GetH() / 2 -  p_cen_y,p_time);
    Zoom(p_zoom,p_time);
}

void Bg::GetPos(int& o_x1,int& o_y1,int& o_x2,int& o_y2){
    o_x1 = m_rect.x;
    o_y1 = m_rect.y;
    o_x2 = m_rect.x + m_rect.w;
    o_y2 = m_rect.y + m_rect.h;
}

void Bg::Load(std::string p_file,int p_x,int p_y,float p_zoom,int p_a){
    //Loading the bacckground
    m_alpha = p_a;
    m_file = p_file;
    m_sur.Load(m_file);
    m_tex.Load((SDL_Surface*)m_sur);
    IsLoaded = true;
    m_WillAlpha = m_WillMove = m_WillZoom = false;

    m_tex.GetSize(m_rect.w,m_rect.h);
    m_src_x = m_rect.x = p_x;
    m_src_y = m_rect.y = p_y;
    m_src_w = m_rect.w * p_zoom ;
    m_src_h = m_rect.h * p_zoom;

    SetDisplayMode();
    m_tex.SetAlpha( m_alpha );
    m_tex.SetPos(m_rect.x,m_rect.y);
    m_tex.SetZoom(m_rect.w,m_rect.h);
}

void Bg::Load(std::string p_file,int p_x,int p_y,int p_w,int p_h,int p_a){
    //Loading the bacckground
    m_src_a = m_alpha = p_a;
    m_file = p_file;
    m_sur.Load(m_file);
    m_tex.Load((SDL_Surface*)m_sur);
    IsLoaded = true;
    m_WillAlpha = m_WillMove = m_WillZoom = false;

    m_src_x = m_rect.x = p_x;
    m_src_y = m_rect.y = p_y;
    m_src_w = m_rect.w = p_w;
    m_src_h = m_rect.h = p_h;

    SetDisplayMode();
    m_tex.SetAlpha( m_alpha );
    m_tex.SetPos(m_rect.x,m_rect.y);
    m_tex.SetZoom(m_rect.w,m_rect.h);
}

void Bg::SetDisplayMode(){
    int m_win_w,m_win_h;
    m_win_w = pRnd.GetW();    //获取窗口宽度
    m_win_h = pRnd.GetH();    //获取窗口长度
    switch(m_dp_mode)
    {
        case CENTER:    //居中
            m_rect.x = (m_win_w-m_rect.w) / 2;
            m_rect.y = (m_win_h-m_rect.h) / 2;
            break;

        case TILE:  //平铺，暂不支持
            break;

        case STRETCH:   //拉伸
            m_rect.x = m_rect.y =0;
            m_rect.w = m_win_w;
            m_rect.h = m_win_h;
            m_tex.SetZoom(m_rect.w,m_rect.h);
            break;

    case FILLED:    //填充
        if( m_rect.w / m_rect.h < m_win_w / m_win_h)
        {
            m_rect.x = 0;
            m_rect.y = ( m_win_h - m_rect.h * m_win_w / m_rect.w ) /2;
            m_rect.h = m_rect.h * m_win_w / m_rect.w ;
            m_rect.w = m_win_w ;
        }else{
            m_rect.x = (m_win_w-m_rect. w * m_win_h / m_rect.h ) / 2 ;
            m_rect.y = 0;
            m_rect.w = m_rect.w * m_win_h / m_rect.h;
            m_rect.h = m_win_h;
        }
        break;

    case ADAPTABLE:
        if( m_rect.w / m_rect.h > m_win_w / m_win_h)
        {
            m_rect.x = 0;
            m_rect.y = ( m_win_h - m_rect.h * m_win_w / m_rect.w) / 2 ;
            m_rect.h = m_rect.h * m_win_w / m_rect.w;
            m_rect.w = m_win_w ;
        }
        else
        {
            m_rect.y = 0;
            m_rect.x = ( m_win_w -m_rect. w * m_win_h / m_rect.h) / 2 ;
            m_rect.w = m_rect.w * m_win_h / m_rect.h;
            m_rect.h = m_win_h ;
        }
        break;

    case DEFAULT:
    default:
        break;
    }
}

void Bg::OnDraw(){
    m_tex.OnDraw();
}

void Bg::OnNext(){
    m_tex.SetAlpha( m_alpha );
    m_tex.SetPos(m_rect.x,m_rect.y);
    m_tex.SetZoom(m_rect.w,m_rect.h);

    if(AfterLoad){
        m_sur.Clear();
        Load(m_file,m_rect.x,m_rect.y,m_rect.w,m_rect.h,m_alpha);
        AfterLoad = false;
    }

    if(m_WillMove){
        m_m_spend = m_m_timer.GetTimer();
        if(m_m_spend < m_m_time){
            m_rect.x  = int(SArcFunc(m_m_spend * 1.0 / m_m_time) * ( m_dst_x - m_src_x) + m_src_x);
            m_rect.y  = int(SArcFunc(m_m_spend * 1.0 / m_m_time) * ( m_dst_y - m_src_y) + m_src_y);
        }else{
            m_src_x = m_rect.x = m_dst_x;
            m_src_y = m_rect.y = m_dst_y;
            m_WillMove = false;
            m_m_timer.Reset();
        }
        SetDisplayMode();
    }

    if(m_WillAlpha){
        m_a_spend = m_a_timer.GetTimer();
        if(m_a_spend < m_a_time){
            m_alpha = int(SArcFunc(m_a_spend * 1.0 / m_a_time) * (m_dst_a - m_src_a) + m_src_a);
        }else{
            m_alpha = m_dst_a;
            m_WillAlpha = false;
            m_a_timer.Reset();
        }
    }

    if(m_WillZoom){
        m_z_spend = m_z_timer.GetTimer();
        if(m_z_spend < m_z_time){
            m_rect.w  = int(SArcFunc(m_z_spend * 1.0 / m_z_time) * ( m_dst_w - m_src_w) + m_src_w);
            m_rect.h  = int(SArcFunc(m_z_spend * 1.0 / m_z_time) * ( m_dst_h - m_src_h) + m_src_h);
        }else{
            m_src_w = m_rect.w = m_dst_w;
            m_src_h = m_rect.h = m_dst_h;
            m_WillZoom = false;
            m_z_timer.Reset();
        }
    }
    SetDisplayMode();
}

void Bg::SMEProc(SMI::SMEvent* pSme){
    if(pSme->cmd == L"bg"){
        if(pSme->argv.size() == 1)
            Load("GalRes/bg/" + WStringToString(pSme->argv[0]) + ".png",0,0,(float)1.0,255);
        else if(pSme->argv.size() == 2)
            Load("GalRes/bg/" + WStringToString(pSme->argv[0]) + ".png",0,0,(float)1.0,_wtoi(pSme -> argv[1].c_str()));
        else if(pSme->argv.size() == 3)
            Load("GalRes/bg/" + WStringToString(pSme->argv[0]) + ".png",_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),(float)1.0,255);
        else if(pSme->argv.size() == 4)
            Load("GalRes/bg/" + WStringToString(pSme->argv[0]) + ".png",_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),(float)_wtoi(pSme -> argv[3].c_str()));
        else
            Load("GalRes/bg/" + WStringToString(pSme->argv[0]) + ".png",_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),_wtoi(pSme -> argv[4].c_str()),_wtoi(pSme -> argv[5].c_str()));
    }else if(pSme->cmd == L"setbgmode"){
        switch(pSme->argv[0][0]){
        case 'A':
            m_dp_mode = ADAPTABLE;break;
        case 'C':
            m_dp_mode = CENTER;break;
        case 'D':
        default:
            m_dp_mode = DEFAULT;break;
        case 'F':
            m_dp_mode = FILLED;break;
        case 'S':
            m_dp_mode = STRETCH;break;
        case 'T':
            m_dp_mode = TILE;break;
        }
        SetDisplayMode();
    }else if(pSme->cmd == L"bg_a"){
        if(pSme->argv.size() == 2)
            Alpha(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()));
        else
            Alpha(_wtoi(pSme -> argv[0].c_str()),0);
    }else if(pSme->cmd == L"bg_m"){
        if(pSme->argv.size() == 3)
            Move(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()));
        else
            Move(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),0);
    }else if(pSme->cmd == L"bg_m2"){
       if(pSme->argv.size() == 3)
            Move_rt(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()));
        else
            Move_rt(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),0);
    }else if(pSme->cmd == L"bg_m_cen"){
        SetCenMove(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),_wtof(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()));
    }else if(pSme->cmd == L"bg_z"){
        if(pSme->argv.size() == 2)
            Zoom((float)_wtof(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()));
        else
            Zoom((float)_wtof(pSme -> argv[0].c_str()),0);
    }else if(pSme->cmd == L"bg_zxy"){
        if(pSme->argv.size() == 3){
            Zoom(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()));
        }else{
            Zoom(_wtoi(pSme -> argv[0].c_str()),_wtoi(pSme -> argv[1].c_str()),0);
        }
    }
}

void Bg::SMEFastProc(SMI::SMEvent* pSme){
    if(m_WillMove){
        m_src_x = m_rect.x = m_dst_x;
        m_src_y = m_rect.y = m_dst_y;
        m_WillMove = false;
        m_a_timer.Reset();
    }

    if(m_WillAlpha){
        m_src_a = m_alpha = m_dst_a;
        m_WillAlpha = false;
        m_a_timer.Reset();
    }

    if(m_WillZoom){
        m_src_w = m_rect.w = m_dst_w;
        m_src_h = m_rect.h = m_dst_h;
        m_WillZoom = false;
        m_z_timer.Reset();
    }
}

bool Bg::SMEFinished(SMI::SMEvent* pSme){
    return (!m_WillAlpha && !m_WillMove && !m_WillZoom);
}

void Bg::SaveData(GameDataMgr::SAVE& bundle){
    bundle.Write<bool>(IsLoaded);
    if(IsLoaded){
        bundle.WriteStr(m_file);
        int tmp_mode;
        tmp_mode = m_dp_mode;
        bundle.Write<int>(tmp_mode);
        bundle.Write<int>(m_rect.x);
        bundle.Write<int>(m_rect.y);
        bundle.Write<int>(m_rect.w);
        bundle.Write<int>(m_rect.h);
        bundle.Write<int>(m_alpha);
    }
}

void Bg::LoadData(GameDataMgr::SAVE& bundle){
    bundle.Read<bool>(IsLoaded);
    if(IsLoaded){
        bundle.ReadStr(m_file);
        int tmp_mode;
        bundle.Read<int>(tmp_mode);
        m_dp_mode = Display_Mode(tmp_mode);
        bundle.Read<int>(m_rect.x);
        bundle.Read<int>(m_rect.y);
        bundle.Read<int>(m_rect.w);
        bundle.Read<int>(m_rect.h);
        bundle.Read<int>(m_alpha);
        AfterLoad = true;
    }
}

bool Bg::FinishAnimation(){
    return !(m_WillAlpha && m_WillMove && m_WillZoom);
}

}
}
