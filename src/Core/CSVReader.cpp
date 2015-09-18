#include "CSVReader.h"
#include "Misc.h"
#include "Debug.h"
#include <cstdlib>
#include <cstdio>

using namespace std;
using namespace Core;

bool CSVReader::LoadCSV(const std::string& f)
{
    ResetPtr();
    m_file.Load(f);
    return m_file.Success();
}

void CSVReader::Close()
{
    m_file.Free();
    ResetPtr();
}

void CSVReader::ResetPtr()
{
    m_ptr = 0;
}

bool CSVReader::NextLine()
{
    if(m_ptr<m_file.Size()) m_nowLine = GetLine(m_file,m_ptr);
    else return false;
    return true;
}


bool CSVReader::PopStr(std::string& s)
{
    if(m_nowLine.empty()) return false;
    else{
        //PNT("NOWLINE"<<m_nowLine);
        auto comma = m_nowLine.find(',');
        if(comma == string::npos){
            s = m_nowLine;
            m_nowLine.clear();
            return true;
        }
        s = m_nowLine.substr(0,comma);
        m_nowLine = m_nowLine.substr(comma+1,m_nowLine.length()-comma-1);
        return true;
    }
}

bool CSVReader::PopInt(int& i)
{
    string s;
    bool isTrue = PopStr(s);
    i = atoi(s.c_str());
    return isTrue;
}

bool CSVReader::PopFloat(double& f)
{
    string s;
    bool isTrue = PopStr(s);
    f = atof(s.c_str());
    return isTrue;
}
