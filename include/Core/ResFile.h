#ifndef _HEAD_RESFILE_HEAD_
#define _HEAD_RESFILE_HEAD_

#include "SDLBase.h"
#include <vector>
#include <string>

namespace Core{

class ResFileReader{
public:
    virtual char* GetFile(const std::string&,Uint32& size) = 0;   //Return nullptr is loaded fail.
};

class LocalFileReader:ResFileReader{
public:
    virtual char* GetFile(const std::string&,Uint32& size);
};

class ResFile
{
public:
    static void InstallReader(ResFileReader*);
    ResFile();
    ResFile(const std::string&);

    ~ResFile();

    void Load(std::string);  //加载文件，不需要考虑是否释放，自动释放
    void Free();    //释放当前文件
    Uint32 Size() const;    //取大小
    bool Success() const;

    operator Uint8* () const;    //取字节型指针
    operator SDL_RWops* ();   //取SDL_RWops指针
    operator void* () const;    //取void*型指针
    operator char* () const;    //取char*型指针

    Uint8& operator [] (Uint32) const;   //使用下标访问某字节
protected:
private:
    void fixPath(std::string&);
    SDL_RWops* m_rw = nullptr;
    char* m_mem = nullptr;
    Uint32 m_size = 0;
    static LocalFileReader m_localLoader;
    static std::vector<ResFileReader*> m_readers;
};
}

#endif // _HEAD_RESFILE_HEAD_
