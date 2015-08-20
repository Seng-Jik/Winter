#ifndef _HEAD_BUNDLE_
#define _HEAD_BUNDLE_
#include "Globals.h"
#include <fstream>
#include "mem.h"

/* 序列化数据包 */
/* 警告！写入或读入超过数据包长度的数据程序会直接崩溃！*/
namespace Core{
    template <Uint32 size>    //传入大小
    class Bundle{
    private:
        Uint8 m_data[size];
        Uint32 m_wptr;
        Uint32 m_rptr;
    public:
        Bundle();
        void ResetPtr();    //重设读写指针
        template <typename type> void Write(const type&);
        //在当前写位置写入一个数据，并把写指针挪向下一个数据
        //仅限基本类型！类似Uint32、BYTE及其已知大小的数组型

        template <typename type> void Read(type&);
        //从当前读位置读出一个数据，并把读指针挪向下一个数据
        //仅限基本类型！类似Uint32、BYTE及其已知大小的数组型

        void WriteStr(const std::string&);    //写入字符串
        void ReadStr(std::string&);     //读出字符串

        void WriteToFile(std::ofstream&,Uint64 pos=0);
        void WriteToFile(std::fstream&,Uint64 pos=0);
        //把数据包存到文件

        void ReadFromFile(std::ifstream&,Uint64 pos=0);
        void ReadFromFile(std::fstream&,Uint64 pos=0);
        //从文件读取数据包

        inline operator void* (){return (void*)&m_data[0];}
        ~Bundle(){};
    };
}

#ifdef _HEAD_BUNDLE_

template<Uint32 size>
Core::Bundle<size>::Bundle()
{
    ResetPtr();
}

template<Uint32 size>
void Core::Bundle<size>::ResetPtr()
{
    m_wptr = 0;
    m_rptr = 0;
}

template<Uint32 size>
template <typename type> void Core::Bundle<size>::Write(const type& data)
{
    memcpy(&m_data[m_wptr],&data,sizeof(type));
    m_wptr += sizeof(type);
}

template<Uint32 size>
template <typename type> void Core::Bundle<size>::Read(type& data)
{
    //data = *(type*)(&m_data[m_rptr]);
    memcpy(&data,&m_data[m_rptr],sizeof(type));
    m_rptr += sizeof(type);
}

template<Uint32 size>
void Core::Bundle<size>::WriteStr(const std::string& data)
{
    Uint16 len = data.length();
    Write<Uint16>(len);
    for(Uint32 i = 0;i < len;++i)
    {
        *(char*)(&m_data[m_wptr]) = data[i] ^ 0xCC;
        m_wptr ++;
    }
}

template<Uint32 size>
void Core::Bundle<size>::ReadStr(std::string& data)
{
    Uint16 len;
    Read<Uint16>(len);
    data.resize(len);
    for(Uint32 i = 0;i < len;++i)
    {
        data[i] = *(char*)(&m_data[m_rptr]) ^ 0xCC;
        m_rptr ++;
    }
}

template<Uint32 size>
void Core::Bundle<size>::WriteToFile(std::ofstream& out,Uint64 pos)
{
    Uint64 p = out.tellp();
    out.seekp(pos);
    out.write((char*)&m_data[0],size);
    out.seekp(p);
}

template<Uint32 size>
void Core::Bundle<size>::ReadFromFile(std::ifstream& in,Uint64 pos)
{
    Uint64 p = in.tellg();
    in.seekg(pos);
    in.read((char*)&m_data[0],size);
    in.seekg(p);
}

template<Uint32 size>
void Core::Bundle<size>::WriteToFile(std::fstream& out,Uint64 pos)
{
    Uint64 p = out.tellp();
    out.seekp(pos);
    out.write((char*)&m_data[0],size);
    out.seekp(p);
}

template<Uint32 size>
void Core::Bundle<size>::ReadFromFile(std::fstream& in,Uint64 pos)
{
    Uint64 p = in.tellg();
    in.seekg(pos);
    in.read((char*)&m_data[0],size);
    in.seekg(p);
}
#endif

#endif // _HEAD_BUNDLE_
