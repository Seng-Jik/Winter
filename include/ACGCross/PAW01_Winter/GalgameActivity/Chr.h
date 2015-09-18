/* ** the  Chr system,using map - stl **
************ Write by Maikaze ************
****https://github.com/amarillys******* */

#pragma once
#include "core.h"
#include "ACGCross/PAW01_Winter/GameDataMgr.h"

namespace ACGCross{
namespace Galgame{

    class Chr{
    private:
        Core::Timer m_a_timer,  //the timer to calculate the time of animation
            m_m_timer,
            m_z_timer;
        Core::Surface m_sur;
        Core::Texture m_tex;

        int m_alpha;
        SDL_Rect m_rect;
        std::string m_file;

        bool AfterLoad;
        bool m_WillMove,m_WillAlpha,m_WillZoom;     //switches
        float m_src_x,m_src_y,m_src_a,m_src_z,m_dst_w,m_dst_h,m_src_w,m_src_h;
        int m_dst_a,m_a_spend,m_a_time,     //the parameter of alpha animation
            m_dst_y,m_dst_x,m_m_spend,m_m_time,     //the parameter of move animation
            m_z_spend,m_z_time;     //the parameter of zoom animation

    public:
        Chr();
        ~Chr();

        void Load(std::string p_chr_file,int p_x = 0,int p_y = 0,float p_zoom = 1.0,int p_a = 255);
        void Load(std::string p_chr_file,int p_x = 0,int p_y = 0,int p_w = 100,int p_h =100,int p_a = 255);
        void Alpha(int p_dst_a = 0,int p_alpha_time = 0);
        void Move(int p_dst_x = 0,int p_dst_y = 0,int p_move_time = 0);
        void Move_rt(int p_dst_x = 0,int p_dst_y = 0,int p_move_time = 0);
        void Zoom(float p_z = 1.0,int p_z_time = 0);
        void Zoom(int p_dst_x,int p_dst_y,int p_z_time =0);
        void GetPos(int &x,int &y){ x = m_rect.x; y = m_rect.y;}
        void OnNext();
        void OnDraw();

        void SaveData(GameDataMgr::SAVE& bundle);
        void LoadData(GameDataMgr::SAVE& bundle);

        void FastProc();
        bool IsFinished();

        bool FinishAnimation();
        void ResetTimer();
    };
}
}
