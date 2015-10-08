/* ** the manager of Chr ,using map - stl **
************ Write by Maikaze ************
****https://github.com/amarillys******* */

#pragma once

#include "SMEProcUnit.h"
#include "Chr.h"
#include "../ACGCross/PAW01_Winter/str2wstr.h"   //to transfomrat the parameter in SMEProc

using namespace ::Snow;
typedef int index;  //[optional]

namespace ACGCross{
    namespace Galgame{

    class ChrMgr:public SMEProcUnit
    {
    private:
        std::map<index,Chr*> m_chr;
        std::map<index,Chr*>::iterator m_it;
        int m_index;    //[Optional] to replace the _wtoi(pSme -> argv[0].c_str()) in SMEProc so that save the time and make it more clearly.
        int m_count;
        bool IsFinished;

        bool m_gaw_hiddenWindow = false;

    public:
        ChrMgr();
        ~ChrMgr();
        bool DetectChr(int p_index,bool p_del); //to detect the existance of the chr and del it[when you need]
        void OnNext();
        void OnDraw();
        void SMEProc(SMI::SMEvent* pSme);
        void SMEFastProc(SMI::SMEvent* pSme);
        bool SMEFinished(SMI::SMEvent* pSme);

        void SaveData(GameDataMgr::SAVE& bundle);
        void LoadData(GameDataMgr::SAVE& bundle);

        bool ChrMgrFinishAnimation();
        void Reset();

    };

    }
}
