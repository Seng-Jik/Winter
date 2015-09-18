#include "ACGCross/PAW01_Winter/GalgameActivity/NameCard.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/TextBox.h"
using namespace ACGCross::Galgame;

void NameCard::SMEFastProc(SMI::SMEvent* e){
    /*if(e -> argv[0] == L"白雪") {
        SetColor(0xF9,0xC2,0xF8);
        m_textBox -> SetColor(0xF9,0xC2,0xF8);
    }
    else if(e -> argv[0] == L"白峰") {
        SetColor(0x95,0xD1,0xF4);
        m_textBox -> SetColor(0x95,0xD1,0xF4);
    }
    SetText(e -> argv[0]);*/
    UpdateTextFast();
};

void NameCard::SMEProc(SMI::SMEvent* e){
    if(e -> argv[0] == L"白雪") {
        SetColor(0xF9,0xC2,0xF8);
        m_textBox -> SetColor(0xF9,0xC2,0xF8);
    }
    else if(e -> argv[0] == L"白峰") {
        SetColor(0x95,0xD1,0xF4);
        m_textBox -> SetColor(0x95,0xD1,0xF4);
    }
    if(m_text == e->argv[0]) return;
    SetText(e -> argv[0]);
    UpdateText();
};

bool NameCard::SMEFinished(SMI::SMEvent*)
{return m_stat == NOTHING;};
void NameCard::SetTextBox(ACGCross::Galgame::TextBox* s){
    m_textBox = s;
};
