#include "ACGCross/PAW01_Winter/GalgameActivity/Chr.h"
#include "ACGCross/PAW01_Winter/MathFunc.h"

using namespace ::Snow;
using namespace std;

namespace ACGCross{
namespace Galgame{

void Chr::Alpha(int p_dst_a,int p_alpha_time){ //the preparation of animation alpha
    m_WillAlpha = true;
    m_dst_a = p_dst_a;
    m_a_time = p_alpha_time;
    m_a_timer.Reset() ;
}

Chr::Chr(){
    AfterLoad = false;
    //cout << "construction" << endl;
}

Chr::~Chr(){
}

void Chr::FastProc(){
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

bool Chr::IsFinished(){ //remained problem
    return true;
    //return m_a_finished && m_m_finished;
}

void Chr::Load(std::string p_chr_file,int p_x,int p_y,float p_zoom,int p_a){
    m_file = p_chr_file;
    m_sur.Load(m_file);
    m_tex.Load((SDL_Surface*)m_sur);
    m_tex.GetSize(m_rect.w,m_rect.h);
    m_src_w = m_rect.w * p_zoom;
    m_src_h = m_rect.h * p_zoom;
    m_src_x = m_rect.x = p_x - m_rect.w / 2;
    m_src_y = m_rect.y = p_y + pRnd.GetH() - m_rect.h;
    m_src_a = m_alpha = p_a;
    m_WillAlpha = m_WillMove = m_WillZoom = false;
    OnNext();
}

void Chr::Load(std::string p_chr_file,int p_x,int p_y,int p_w,int p_h,int p_a){
    m_file = p_chr_file;
    m_sur.Load(m_file);
    m_tex.Load((SDL_Surface*)m_sur);
    m_src_w = m_rect.w = p_w;
    m_src_h = m_rect.h = p_h;
    m_src_x = m_rect.x = p_x - m_rect.w / 2;
    m_src_y = m_rect.y = p_y + pRnd.GetH() - m_rect.h;
    m_src_a = m_alpha = p_a;
    m_WillAlpha = m_WillMove = m_WillZoom = false;
    OnNext();
}

void Chr::Move(int p_dst_x,int p_dst_y,int p_move_time){    //the preparation of animation move
    m_WillMove = true;
    m_dst_x = p_dst_x = p_dst_x - m_rect.w / 2;
    m_dst_y = p_dst_y  = p_dst_y + pRnd.GetH() - m_rect.h;
    m_m_time = p_move_time;
    m_m_timer.Reset();
}

void Chr::Move_rt(int p_dst_x,int p_dst_y,int p_move_time){    //the preparation of animation move
    m_WillMove = true;
    m_dst_x = p_dst_x + m_rect.x;
    m_dst_y  = p_dst_y + m_rect.y;
    m_m_time = p_move_time;
    m_m_timer.Reset();
}

void Chr::OnNext(){
    if(AfterLoad){
        Load(m_file,m_rect.x,m_rect.y,m_rect.w,m_rect.h,m_alpha);
        AfterLoad = false;
    }
    m_tex.SetAlpha( m_alpha );
    m_tex.SetPos(m_rect.x,m_rect.y);
    m_tex.SetZoom(m_rect.w,m_rect.h);

    if(m_WillMove){
        m_m_spend = m_m_timer.GetTimer();
        if( m_m_spend < m_m_time){
            m_rect.x  = int(SArcFunc(m_m_spend * 1.0 / m_m_time) * ( m_dst_x - m_src_x) + m_src_x);
            m_rect.y  = int(SArcFunc(m_m_spend * 1.0 / m_m_time) * ( m_dst_y - m_src_y) + m_src_y);
        }else{
            m_rect.x = m_dst_x;
            m_rect.y = m_dst_y;
            m_WillMove = false;
            m_m_timer.Reset();
        }
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
}

void Chr::OnDraw(){
    m_tex.OnDraw();
}

void Chr::SaveData(GameDataMgr::SAVE& bundle){
    bundle.WriteStr(m_file);
    bundle.Write<int>(m_rect.x);
    bundle.Write<int>(m_rect.y);
    bundle.Write<int>(m_rect.w);
    bundle.Write<int>(m_rect.h);
    bundle.Write<int>(m_alpha);
}

void Chr::LoadData(GameDataMgr::SAVE& bundle){
    bundle.ReadStr(m_file);
    bundle.Read<int>(m_rect.x);
    bundle.Read<int>(m_rect.y);
    bundle.Read<int>(m_rect.w);
    bundle.Read<int>(m_rect.h);
    bundle.Read<int>(m_alpha);
    AfterLoad = true;
}

void Chr::Zoom(float p_z,int p_z_time){
    m_WillZoom = true;
    m_dst_w = m_src_w * p_z;
    m_dst_h = m_src_h * p_z;
    m_z_time = p_z_time;
    m_z_timer.Reset();
}

void Chr::Zoom(int p_dst_w,int p_dst_h,int p_z_time){
    m_WillZoom = true;
    m_dst_w = p_dst_w;
    m_dst_h = p_dst_h;
    m_z_time = p_z_time;
    m_z_timer.Reset();
}

bool Chr::FinishAnimation(){
    return !(m_WillAlpha && m_WillMove && m_WillZoom);
}

void Chr::ResetTimer(){
    m_a_timer.Reset();
    m_m_timer.Reset();
    m_z_timer.Reset();
}

    }
}
