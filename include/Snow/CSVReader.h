#pragma once
#include "ResFile.h"
#include <string>

namespace Snow{
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
        Snow::ResFile m_file;
        Uint32 m_ptr;
        std::string m_nowLine;
};
}

