#ifndef CGVIEWER_H
#define CGVIEWER_H

#include "Snow.h"

namespace ACGCross{
namespace Galgame{

class CGViewer : public ::Snow::Activity
{
    public:
        CGViewer();
        virtual ~CGViewer();
        virtual void OnShow();
        virtual void OnHide();
        virtual void OnInit();
        virtual void OnDraw();
        virtual void OnEvent(const SDL_Event&);
        virtual void OnNext();
    protected:
    private:
};

}
}

#endif // CGVIEWER_H
