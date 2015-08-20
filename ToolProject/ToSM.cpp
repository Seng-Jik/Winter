#include <iostream>
#include <fstream>
#include <string>
#include <mem.h>

using namespace std;

std::wstring  StringToWString(const std::string& s)
{
    setlocale(LC_ALL, "chs");
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

std::string WStringToString(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

    setlocale(LC_ALL, "chs");

    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;

    setlocale(LC_ALL, curLocale.c_str());

    return result;
}


int main()
{
    ifstream in("sm.txt",ios::binary);
    ofstream out("winter_chs.smo",ios::binary);
    unsigned int lines = 0;

    while(!in.eof()){
        string s;
        getline(in,s);
        wstring ws = StringToWString(s);

        if(s.empty()){
            out.put('\r');
            out.put('\n');
            continue;
        }
        else if(ws.find(L"：") == wstring::npos){
            for(auto p = s.begin();p != s.end();++p){
                if(*p != 0x0D) out.put(*p);
            }
        }else{
            string name,text;
            name = WStringToString(ws.substr(0,ws.find(L"：")));
            text =  WStringToString(ws.substr(ws.find(L"：")+1,ws.length()-1-ws.find(L"：")));
            out.put('[');
            for(auto p = name.begin();p != name.end();++p)
                if(*p != 0x0D) out.put(*p);
            out.put(']');
            for(auto p = text.begin();p != text.end();++p)
                if(*p != 0x0D) out.put(*p);
        };
        out.put('\\');
        //out.put(' ');
        out.put('\r');
        out.put('\n');
        //out.put('\r');
        //out.put('\n');
        lines++;
    }
    cout<<"Finished!\nLines:"<<lines<<endl;
    system("pause");
    return 0;
}

