#ifndef _HEAD_NAMECARD_HEAD_
#define _HEAD_NAMECARD_HEAD_

#include "ACGCross/PAW01_Winter/FadeLable.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/SMEProcUnit.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/TextBox.h"
#include "SMLanguage/SMI.h"

namespace ACGCross{
namespace Galgame{
class TextBox;
class NameCard:public ACGCross::FadeLable,public ACGCross::Galgame::SMEProcUnit
{
    public:
        void SMEFastProc(SMI::SMEvent*);
        void SMEProc(SMI::SMEvent* e);
        bool SMEFinished(SMI::SMEvent*);
        void SetTextBox(ACGCross::Galgame::TextBox*);
        void Clear();
    protected:
        TextBox* m_textBox;
    private:
};

}
}

#endif // _HEAD_NAMECARD_HEAD_
