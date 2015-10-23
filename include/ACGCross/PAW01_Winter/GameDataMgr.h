#pragma once
#include "Snow.h"
#include <fstream>
#include <set>
#include <bitset>
namespace ACGCross{

class GameDataMgr   //Thread Safe
{
    public:
        std::bitset<65536> kidoku;
        typedef ::Snow::Bundle<65536> SAVE;
        GameDataMgr();

        bool GetSaveExist();    //人工存档是否存在
        bool GetCSaveExist();   //自动存档是否存在
        bool GetCGExist();  //CG是否存在

        void AddUpdateTask(int num);    //添加更新存档的任务，>=0为存档更新，-1为CG状态，-2为系统状态，-3为设置
        void UpdateData();  //执行更新

        bool CGOpend(int num);
        void OpenCG(int num);

        void Lock();
        void Unlock();
        SAVE& operator [] (int);    //操作前Lock，完成后Unlock。
        bool GetDataExist(int num);
        void SetDataExist(int num,bool tf);

        //inline bool GetBgmSetting(){return m_cfg_bgm;}
        //inline bool GetSeSetting(){return m_cfg_se;}
        //inline bool GetCVSetting(){return m_cfg_cv;}
        inline Uint8 GetCVVol(){return m_cfg_cvVol;}
        inline Uint8 GetBGMVol(){return m_cfg_bgmVol;}
        inline Uint8 GetSEVol(){return m_cfg_seVol;}
        inline void SetBGMVol(Uint8 v){m_cfg_bgmVol = v;}
        inline void SetSEVol(Uint8 v){m_cfg_seVol = v;}
        inline void SetCVVol(Uint8 v){m_cfg_cvVol = v;}
        inline void SetGrpMode(Uint8 g){m_cfg_grpMode = g;}
        inline Uint8 GetGrpMode(){return m_cfg_grpMode;}
        inline bool Get16_9(){return m_cfg_grpMode >= 5;}
    protected:
    private:

        //bool m_cfg_cv;
        //bool m_cfg_bgm;
        //bool m_cfg_se;
        Uint8 m_cfg_grpMode;
        Uint8 m_cfg_cvVol,m_cfg_seVol,m_cfg_bgmVol;

        SAVE m_save[16];
        bool m_dataExist[16];
        bool m_cgState[8*32];

        ::Snow::Mutex m_mtx;

        std::set<int> m_updateTask;


};
}

