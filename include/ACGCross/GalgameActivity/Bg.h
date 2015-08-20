/* ** the  Bg system,using map - stl **
************ Write by Maikaze ************
****https://github.com/amarillys******* */
#pragma once

#include "SMEProcUnit.h"
#include "../ACGCross/str2wstr.h"
#include "../../core.h"
#include "ACGCross/GameDataMgr.h"

namespace ACGCross{
namespace Galgame{
    enum Display_Mode{
            DEFAULT = 0,
            CENTER , //居中
            TILE,   //平铺
            STRETCH,    //拉伸
            FILLED, //填充
            ADAPTABLE   //适应
    };

    class Bg:public SMEProcUnit
    {
    private:
        Core::Timer m_a_timer,
            m_m_timer,m_z_timer;
        int m_a_time,m_m_time,m_z_time,
            m_a_spend,m_m_spend,m_z_spend,
            m_dst_w,m_dst_h,m_dst_x,m_dst_y,m_dst_a;
        float m_src_x,m_src_y,m_src_a,m_src_w,m_src_h;
        bool m_WillAlpha,m_WillMove,m_WillZoom;
        bool AfterLoad,IsLoaded;

        Core::Surface m_sur;
        Core::Texture m_tex;

        Display_Mode m_dp_mode;
        int m_eff_index;
        int m_alpha;
        SDL_Rect m_rect;
        std::string m_file;

    public:
        Bg();
        ~Bg();
        void Load(std::string p_file,int p_x = 0,int p_y = 0,float p_zoom = 1.0,int p_a = 255);
        void Load(std::string p_file,int p_x = 0,int p_y = 0,int p_w = 0,int p_h = 0,int p_a = 255);
        void OnDraw();
        void OnNext();

        void SMEFastProc(SMI::SMEvent* pSme);
        bool SMEFinished(SMI::SMEvent* pSme);
        void SMEProc(SMI::SMEvent* pSme);

        SDL_Surface* GetSurface();
        void GetPos(int& o_x1,int& o_y1,int& o_x2,int& o_y2);   //leftup rightdown
        void Alpha(int p_a = 0,int p_a_time = 0);
        void Move(int p_dst_x = 0,int p_dst_y = 0,int p_m_time = 0);
        void Move_rt(int p_dst_x = 0,int p_dst_y = 0,int p_m_time = 0);
        void Zoom(float p_z = 0,int p_z_time = 0);
        void Zoom(int p_dst_x,int p_dst_y,int p_z_time = 0);
        void SetCenMove(int cen_x = 0,int cen_y = 0,float zoom = 1.0,int time = 0);
        void SetDisplayMode();

        void SaveData(GameDataMgr::SAVE& bundle);
        void LoadData(GameDataMgr::SAVE& bundle);

        bool FinishAnimation();

    };
}
}
