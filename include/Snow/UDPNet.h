#pragma once
#include <string>
#include "SDLbase.h"
namespace Snow{

class UDPPacket;
class UDPSocket
{
    public:
        bool Open(Uint16 port);
        void Close();
        bool Opened();

        int Send(const UDPPacket&);
        int Recv(UDPPacket&);
    protected:
    private:
        UDPsocket m_socket = nullptr;
};

class UDPPacket{
friend class UDPSocket;
private:
    UDPpacket* m_pak;
public:
    UDPPacket(int size);
    ~UDPPacket();
    void SetDst(const std::string& ip,Uint16 port);
    std::string GetIP();

    inline Uint16 GetPort();

    inline operator void* (){
        return m_pak -> data;
    }
    inline int Size(){
        return m_pak -> len;
    };
};

}
