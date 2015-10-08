#include "Snow.h"
#include <map>
namespace ACGCross{
class DASPReader : public ::Snow::ResFileReader
{
    public:
        void OpenDASP(const char*);
        void SetDic(const void*);
        virtual char* GetFile(const std::string&,Uint32& size);
    protected:
    private:
        struct FPoi{
            Uint64 beg,len;
        };
        std::map<std::string,FPoi> m_fs;
        std::ifstream m_dasp;
        const void* m_dic;
        char m_idx[256];
};
}
