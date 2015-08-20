#include "Core/Font.h"

using namespace Core;
using namespace std;

Font::Font()
{
    m_font = nullptr;
}

Font::Font(const std::string& file,int pt)
{m_font = nullptr;Open(file,pt);}

void Font::Open(const std::string& file,int pt)
{
    if(m_font != nullptr) TTF_CloseFont(m_font);
    m_file.Load(file);
    m_font = TTF_OpenFontRW((SDL_RWops*)m_file,m_file.Size(),pt);
}

Font::operator TTF_Font* ()
{return m_font;}

Font::~Font()
{if(m_font != nullptr) TTF_CloseFont(m_font);}

int Font::GetCharWei(const char ch)
{
    const char s[2] = {ch,'\0'};
    int w;
    TTF_SizeUTF8(m_font,s,&w,nullptr);
    return w;
}

int Font::GetCharWei(const wchar_t ch)
{
    const wchar_t ws[2] = {ch,L'\0'};
    int w=0,h=0;
    TTF_SizeUNICODE(m_font,(Uint16*)ws,&w,&h);
    return w;
}

