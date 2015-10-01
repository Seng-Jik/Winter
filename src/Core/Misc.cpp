#include "Core/Misc.h"
#include "Core/SDLbase.h"
#include "Core/Activity.h"
#include "Core/Globals.h"
#include "Core/RndPtr.h"

#include <set>
#include <stack>
#include <list>

using namespace std;
using namespace Core;


void Core::GetString(std::ifstream& in,std::string& str)
{
    str = "";
    while(1)
    {
        char ch = in.get();
        if(ch == '\0') break;
        else str += ch;
    }
}

void Core::Pos(int& opr_x,int& opr_y,
                        float x,float y)
{
    opr_x = pRnd.GetW() * x;
    opr_y = pRnd.GetH() * y;
}

void Core::Pos(int& opr_x,int& opr_y,
                        const char base,int x,int y)
{
    if((base & 0xF0) == 0xA0) opr_x = pRnd.GetW()/3;
    else if((base & 0xF0) == 0xB0) opr_x = pRnd.GetW()/3 * 2;
    else opr_x = (pRnd.GetW()/4) * ((base & 0xF0) / 0x10) + x;

    if((base & 0x0F) == 0x0A) opr_y = pRnd.GetH()/3;
    else if((base & 0x0F) == 0x0B) opr_y = pRnd.GetH()/3 * 2;
    else opr_y = (pRnd.GetH()/4) * (base & 0x0F) + y;
}

bool Core::InRect(const SDL_Rect& r,int x,int y)
{
    return
        x >= r.x && x <= r.x + r.w &&
        y >= r.y && y <=r.y + r.h;
}

void Core::Activity::RegControl(Control& c)
{m_ansList.push_front(&c);}

Uint32 Time2Fps(Uint32 ms)
{
    return float(ms)/1000 * FPS;
}

void Core::RndPtr::Clear(){
    SDL_Rect r = {0,0,pRnd.GetW(),pRnd.GetH()};
    SDL_RenderFillRect(m_rnd,&r);
}

void Core::UpperCase(std::string& s){
    FOR_EACH(p,s.begin(),s.end())
        if(*p>='a' && *p<='z') *p-='a'-'A';
}

string Core::GetLine(ResFile& r,Uint32& pos){
    string Line;
    for(;pos<r.Size() && r[pos] != '\n';++pos)
        Line+=r[pos];
    ++pos;
    return Line;
}

Uint32 Core::Time2Fps(unsigned int ms){
    return Uint32(float(ms) / 1000 * FPS);
}


//#define DTO_isBackColor(poi,sur) *((Uint8*)sur -> pixels + sur ->pitch*poi.y + poi.x*4) == 0x00
#define DTO_PutNewPx(P,u) *(Uint32*)(newPix + sur->pitch*P.y +P.x*4) = u

bool DTO_isBackColor(SDL_Point& poi,SDL_Surface* sur){
    //PNT("DTO:"<<(Uint16)*((Uint8*)sur -> pixels + sur ->pitch*poi.y + poi.x*4+3));
    return *((Uint8*)sur -> pixels + sur ->pitch*poi.y + poi.x*4+3) < 0xff;
}

void Core::DrawTextOutLine(SDL_Surface* sur)
{
    SDL_LockSurface( sur );
    Uint8* orgPix = (Uint8*)sur ->pixels;
    Uint8* newPix =new Uint8 [sur ->pitch * sur ->h];
    memset((void*)newPix,0,sur->h*sur->pitch);
    //Create Shadow

    SDL_Point poi;
    for(poi.y=0;poi.y<sur->h;++poi.y)
    for(poi.x = 0;poi.x<sur->w;++poi.x){
            Uint32 px = *(Uint32*)(orgPix + sur->pitch*poi.y +poi.x*4);
            if(px>>24 > 0){
                //»æÖÆÒõÓ°
                SDL_Point pShadowPx = poi;
                pShadowPx.x+=1;
                pShadowPx.y+=1;

                if(!(pShadowPx.x >= sur ->w || pShadowPx.y >= sur ->h))
                    DTO_PutNewPx(pShadowPx,0x44666666);

                pShadowPx.x-=2;
                pShadowPx.y-=2;
                DTO_PutNewPx(pShadowPx,px);
            }
    }
    memcpy(sur->pixels,(void*)newPix,sur->h*sur->pitch);
    delete [] newPix;
    SDL_UnlockSurface( sur );
}

void Core::FastBlurARGB8888(SDL_Surface* pOrg,int radius){
    SDL_LockSurface(pOrg);
      if (radius<1){
        return;
        SDL_UnlockSurface(pOrg);
      }
      int w=pOrg ->w;
      int h=pOrg -> h;
      int wm=w-1;
      int hm=h-1;
      int wh=w*h;
      int div=radius+radius+1;
      int* r=new int[wh];
      int* g=new int[wh];
      int* b=new int[wh];
      int rsum,gsum,bsum,x,y,i,p,p1,p2,yp,yi,yw;
      int* vmin = new int[max(w,h)];
      int* vmax = new int[max(w,h)];
      int* pix=(int*)pOrg -> pixels;
      int* dv=new int[256*div];
      for (i=0;i<256*div;i++){
         dv[i]=(i/div);
      }

      yw=yi=0;

      for (y=0;y<h;y++){
        rsum=gsum=bsum=0;
        for(i=-radius;i<=radius;i++){
          p=pix[yi+min(wm,max(i,0))];
          rsum+=(p & 0xff0000)>>16;
          gsum+=(p & 0x00ff00)>>8;
          bsum+= p & 0x0000ff;
       }
        for (x=0;x<w;x++){

          r[yi]=dv[rsum];
          g[yi]=dv[gsum];
          b[yi]=dv[bsum];

          if(y==0){
            vmin[x]=min(x+radius+1,wm);
            vmax[x]=max(x-radius,0);
           }
           p1=pix[yw+vmin[x]];
           p2=pix[yw+vmax[x]];

          rsum+=((p1 & 0xff0000)-(p2 & 0xff0000))>>16;
          gsum+=((p1 & 0x00ff00)-(p2 & 0x00ff00))>>8;
          bsum+= (p1 & 0x0000ff)-(p2 & 0x0000ff);
          yi++;
        }
        yw+=w;
      }

      for (x=0;x<w;x++){
        rsum=gsum=bsum=0;
        yp=-radius*w;
        for(i=-radius;i<=radius;i++){
          yi=max(0,yp)+x;
          rsum+=r[yi];
          gsum+=g[yi];
          bsum+=b[yi];
          yp+=w;
        }
        yi=x;
        for (y=0;y<h;y++){
          pix[yi]=0xff000000 | (dv[rsum]<<16) | (dv[gsum]<<8) | dv[bsum];
          if(x==0){
            vmin[y]=min(y+radius+1,hm)*w;
            vmax[y]=max(y-radius,0)*w;
          }
          p1=x+vmin[y];
          p2=x+vmax[y];

          rsum+=r[p1]-r[p2];
          gsum+=g[p1]-g[p2];
          bsum+=b[p1]-b[p2];

          yi+=w;
        }
      }

    delete []r;
    delete []g;
    delete []b;
    delete []vmin;
    delete []vmax;
    delete []dv;

    SDL_UnlockSurface(pOrg);
}
