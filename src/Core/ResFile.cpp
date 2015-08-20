#include "ResFile.h"
#include "Error.h"
#include <fstream>

using namespace Core;
using namespace std;

LocalFileReader Core::ResFile::m_localLoader;
std::vector<ResFileReader*> Core::ResFile::m_readers;

void ResFile::fixPath(string& path)
{
    for(unsigned int i = 0;i < path.length();++i){
        if(path[i] == '/') path[i] = '\\';
        else if(path[i] >= 'a' && path[i] <= 'z') path[i] += 'A' - 'a';
    }
}

char* LocalFileReader::GetFile(const string& f,Uint32& size)
{
    ifstream in(f.c_str(),ios::binary);
    if(!in) return nullptr;

    in.seekg(0,ios::end);
    size = in.tellg();
    in.seekg(0,ios::beg);

    char* mem = nullptr;
    try{
        mem = new char[size];
    }catch(bad_alloc){
        throw Error(0x30000001,"Can't open local file " + f +".");
    }

    in.read(mem,size);
    return mem;
}

void ResFile::InstallReader(ResFileReader* r){
    m_readers.push_back(r);
}

ResFile::ResFile(){};

ResFile::ResFile(const std::string& s)
{
    Load(s);
}

ResFile::~ResFile()
{
    Free();
}

void ResFile::Load(std::string f)
{
    fixPath(f);
    Uint32 size;
    char* mem = m_localLoader.GetFile(f,size);
    if(mem != nullptr){
        m_mem = mem;
        m_size = size;
        return;
    }else{
        for(auto p = m_readers.begin();p != m_readers.end();++p){
            mem = (*p) -> GetFile(f,size);
            if(mem != nullptr){
                m_mem = mem;
                m_size =  size;
                return;
            }
        }
    }
    throw Error(0x30000001,"Can't open file " + f +".");
}

void ResFile::Free()
{
    if(m_rw != nullptr)
    {
        SDL_FreeRW(m_rw);
        m_rw = nullptr;
        //PNT("SDLRWFree:"<<dbg_info<<std::endl);
    }
    if(m_mem != nullptr)
    {
        delete [] m_mem;
        m_mem = nullptr;
       // PNT("SDLSPACEFree:"<<dbg_info<<std::endl);
    }
    m_size = 0;
}

Uint32 ResFile::Size() const
{
    return m_size;
}

bool ResFile::Success() const
{
    return m_mem != nullptr;
}

ResFile::operator Uint8*() const
{
    return (Uint8*)m_mem;
}

ResFile::operator SDL_RWops*()
{
    if(m_rw == nullptr){
        m_rw = SDL_RWFromMem((void*)m_mem,m_size);
    }
    return m_rw;
}

ResFile::operator void*() const
{
    return (void*)m_mem;
}

ResFile::operator char*() const
{
    return m_mem;
}

Uint8& ResFile::operator[](Uint32 s) const
{
    return *((Uint8*)m_mem+s);
}
