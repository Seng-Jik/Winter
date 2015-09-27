#include "Core/RenderTarget.h"
#include "Core/Globals.h"
 using namespace Core;

RenderTarget::RenderTarget(int w, int h)
{
    m_target = SDL_CreateTexture(pRnd,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_TARGET,w,h);
    m_drawArea.x = m_drawArea.y = 0;
    m_drawArea.w = w;m_drawArea.h = h;
    m_drawPos.x = m_drawPos.y = 0;
    m_drawPos.w = w;m_drawPos.h = h;
}

RenderTarget::~RenderTarget()
{
    SDL_DestroyTexture(m_target);
}

bool RenderTarget::Supposed()
{
    return SDL_RenderTargetSupported(pRnd);
}

void RenderTarget::SetDrawPos(int x, int y, int w, int h)
{
    m_drawPos.x = x;
    m_drawPos.y = y;
    m_drawPos.w = w;
    m_drawPos.h = h;
}

void RenderTarget::SetDrawArea(int x, int y, int w, int h)
{
    m_drawArea.x = x;
    m_drawArea.y = y;
    m_drawArea.w = w;
    m_drawArea.h = h;
}

void RenderTarget::DrawToScreen()
{
    SDL_RenderCopy(pRnd,m_target,&m_drawArea,&m_drawPos);
}

void RenderTarget::Begin()
{
    SDL_SetRenderTarget(pRnd,m_target);
}

void RenderTarget::End()
{
    SDL_SetRenderTarget(pRnd,nullptr);
}

RenderTarget::operator SDL_Texture*()
{
    return m_target;
}

SoftLinkTarget::SoftLinkTarget(int w, int h, Uint32 sdlPixelFormat)
{
    int bpp;
    Uint32 rm,gm,bm,am;
    SDL_PixelFormatEnumToMasks(sdlPixelFormat,&bpp,&rm,&gm,&bm,&am);
    m_frameBuf = new Uint8 [w*h*bpp];
    m_pitch = w*bpp;
    m_target = SDL_CreateTexture(pRnd,sdlPixelFormat,SDL_TEXTUREACCESS_STREAMING,w,h);
    m_mut = nullptr;

    m_drawArea.x = m_drawArea.y = 0;
    m_drawArea.w = w;m_drawArea.h = h;
    m_drawPos.x = m_drawPos.y = 0;
    m_drawPos.w = w;m_drawPos.h = h;
}

void SoftLinkTarget::SetMutex(Mutex* m)
{
    m_mut = m;
}

SoftLinkTarget::operator void*()
{
    return m_frameBuf;
}

int SoftLinkTarget::Pitch()
{
    return m_pitch;
}

void SoftLinkTarget::Update()
{
    SDL_UpdateTexture(m_target,&m_drawArea,m_frameBuf,m_pitch);
}

void SoftLinkTarget::DrawToScreen()
{
    if(m_mut != nullptr) m_mut -> Lock();
    SDL_RenderCopy(pRnd,m_target,&m_drawArea,&m_drawPos);
    if(m_mut != nullptr) m_mut -> Unlock();
}

void SoftLinkTarget::SetDrawPos(int x, int y, int w, int h)
{
    m_drawPos.x = x;
    m_drawPos.y = y;
    m_drawPos.w = w;
    m_drawPos.h = h;
}

void SoftLinkTarget::SetDrawArea(int x, int y, int w, int h)
{
    m_drawArea.x = x;
    m_drawArea.y = y;
    m_drawArea.w = w;
    m_drawArea.h = h;
}

SoftLinkTarget::operator SDL_Texture*()
{
    return m_target;
}
