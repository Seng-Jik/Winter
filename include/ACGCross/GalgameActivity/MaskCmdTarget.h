#ifndef MASKCMDTARGET_H
#define MASKCMDTARGET_H
#include "ACGCross/Mask.h"
#include "bg.h"
#include "ACGCross/GalgameActivity/SMEProcUnit.h"

namespace ACGCross{
    namespace Galgame{

class MaskCmdTarget:public SMEProcUnit
{
    public:
        MaskCmdTarget(Bg*);

        void SMEFastProc(SMI::SMEvent*);
        bool SMEFinished(SMI::SMEvent*);
        void SMEProc(SMI::SMEvent*);

        void LoadMask(const std::string&);
        void DoMask(const std::string&,int cen_x,int cen_y,int alp,float zoom,int time);

        void StopMask();

        void OnDraw();
        void OnNext();

        ~MaskCmdTarget();
    protected:
    private:
        enum{
            MASKING_FR,
            NOTHING
        }m_stat = NOTHING;
        Bg* m_bg;
        Bg m_newBg;
        SDL_Surface* m_newBgSur = nullptr;
        SDL_Surface* m_white,*m_black;
        Core::Texture m_tex;
        ACGCross::Mask m_m;
        Core::Timer m_timer;
        int m_time;

        std::string m_arg_file;
        int m_arg_cenx,m_arg_ceny,m_arg_alp;
        float m_arg_zoom;
};
}
}

#endif // MASKCMDTARGET_H
