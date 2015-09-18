#ifndef _HEAD_CSVREADER_HEAD_
#define _HEAD_CSVREADER_HEAD_
#include "ResFile.h"
#include <string>

namespace Core{
class CSVReader
{
    public:
        bool LoadCSV(const std::string&);
        void Close();
        void ResetPtr();
        bool PopStr(std::string&);
        bool PopInt(int&);
        bool PopFloat(double&);
        bool NextLine();
    protected:
    private:
        Core::ResFile m_file;
        Uint32 m_ptr;
        std::string m_nowLine;
};
}

#endif // _HEAD_CSVREADER_HEAD_
