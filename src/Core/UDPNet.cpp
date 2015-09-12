#include "UDPNet.h"

using namespace Core;

bool UDPSocket::Open(Uint16 port)
{
    m_socket = SDLNet_UDP_Open(port);
    return m_socket != nullptr;
}

void UDPSocket::Close()
{
    if(Opened()) SDLNet_UDP_Close(m_socket);
    m_socket = nullptr;
}

bool UDPSocket::Opened()
{
    return m_socket != nullptr;
}

int UDPSocket::Send(const UDPPacket& p)
{
    return SDLNet_UDP_Send(m_socket,-1,p.m_pak);
}

int UDPSocket::Recv(UDPPacket& p)
{
    return SDLNet_UDP_Recv(m_socket,p.m_pak);
}


UDPPacket::UDPPacket(int size)
{
    m_pak = SDLNet_AllocPacket(size);
}

UDPPacket::~UDPPacket()
{
    SDLNet_FreePacket(m_pak);
}

void UDPPacket::SetDst(const std::string& ip, Uint16 port)
{
    SDLNet_ResolveHost(&(m_pak->address),ip.c_str(),port);
}

std::string UDPPacket::GetIP()
{
    return std::string(SDLNet_ResolveIP(&(m_pak->address)));
}

Uint16 UDPPacket::GetPort()
{
    return m_pak ->address.port;
}
