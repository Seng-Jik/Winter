#pragma once
#include "ResFile.h"

namespace Core{

class Surface{
private:
	SDL_Surface* m_sur = nullptr;
	ResFile m_file;

public:
    Surface();
    Surface(SDL_Surface*);
    Surface(const std::string& f);

    void ConvertFormat(Uint32 sdlFormat);   //转换格式

    void SetAlpha(Uint8 alp);
    void SetBlend(SDL_BlendMode blendMode);
    void SetColor(Uint8 r,Uint8 g,Uint8 b);



	void Load(const std::string& f);  //从这个接口加载的默认会打开RLE优化并使用BLEND混合。
	void Load(SDL_Surface* p);
	void Clear();
	inline operator SDL_Surface* (){
		return m_sur;
	}

	void Lock();
	void Unlock();
	inline void PSet(int x,int y,Uint32 pxARGB8888){
        *(Uint32*)(((Uint8*)m_sur -> pixels) + y * m_sur ->pitch +x*4) = pxARGB8888;
	}

	inline Uint32 PGet(int x,int y){
        return *(Uint32*)(((Uint8*)m_sur -> pixels) + y * m_sur ->pitch +x *4);
	}

	inline Uint32* GetPixels(){return (Uint32*)m_sur -> pixels;}
	inline int GetPitch(){return m_sur -> pitch;}

	void GetSize(int& w,int& h){w = m_sur ->w;h = m_sur ->h;}
	void BlitWithSubSurface(Surface& src,SDL_Rect& srcd,SDL_Rect& dst);

	~Surface(){
		Clear();
	}
};
}
