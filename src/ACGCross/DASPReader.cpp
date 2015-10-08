#include "DASPReader.h"
#include "DAS_Decoder.h"

using namespace ACGCross;
using namespace ::Snow;
using namespace std;

void DASPReader::OpenDASP(const char* _)
{
    m_dasp.open(_,ios::binary);
    if(!m_dasp.good()) {
        ERROR_MSGBOX("Can't open dasp packet.");
        Exit(-1);
    }

    m_dasp.seekg(-8,ios::end);
    Uint64 fileCount;
    m_dasp.read((char*)&fileCount,8);

    m_dasp.seekg(-(8+64*fileCount),ios::end);
    struct FGrid{
        Uint64 beg,len;
        char name[48];
    };
    for(unsigned int i = 0;i < fileCount;++i){
        FGrid fg;
        m_dasp.read((char*)&fg,64);
        string name(fg.name);
        FPoi fp = {fg.beg,fg.len};
        m_fs[name] = fp;
    }
}

void DASPReader::SetDic(const void* _)
{
    m_dic = _;
    DAS_CreateDictionaryIndex(m_dic,m_idx);
}

char* DASPReader::GetFile(const std::string& _, Uint32& size)
{
    if(m_fs.count(_) == 0) return nullptr;
    FPoi& poi = m_fs[_];
    size = poi.len;
    m_dasp.seekg(poi.beg,ios::beg);
    char* pMem = new char[size];
    m_dasp.read(pMem,size);
    DAS_Decode(m_idx,m_dic,pMem,pMem,size);
    return pMem;
}
