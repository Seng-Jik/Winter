#pragma once
#include "Snow.h"
#include "SMLanguage/SMI.h"

namespace ACGCross{
namespace Galgame{

class GalgameActivity;

class SMEProcUnit{
public:
    virtual void SMEProc(SMI::SMEvent*) = 0;
    virtual void SMEFastProc(SMI::SMEvent*) = 0;
    virtual bool SMEFinished(SMI::SMEvent*){return true;}
};

}
}
