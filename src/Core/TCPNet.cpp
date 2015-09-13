#include "Core/TCPNet.h"
using namespace Core;

bool TCPConnection::ConnectToServer(const std::string host,Uint16 port){
	if(SDLNet_ResolveHost(&m_ip,host.c_str(),port) == -1){
        m_socket = nullptr;
        return false;
	}
	m_socket = SDLNet_TCP_Open(&m_ip);
	if(m_socket == nullptr){
		return false;
	}
	return true;
}

void TCPConnection::Close()
{
    if(m_socket != nullptr) SDLNet_TCP_Close(m_socket);
    m_socket = nullptr;
}

bool TCPConnection::Connected()
{
    return m_socket != nullptr;
}


int TCPConnection::Send(const void* m,int size){
	return SDLNet_TCP_Send(m_socket,m,size);
}

int TCPConnection::Recv(void* m,int size){
	return SDLNet_TCP_Recv(m_socket,m,size);
}


int TCPConnection::WaitRecv(void* m,int size){
	int ret;
	while(1){
		ret = Recv(m,size);
		if(ret > 0) return ret;
		SDL_Delay(20);
	}
}

TCPConnection::~TCPConnection(){
	Close();
}

TCPServer::TCPServer(Uint16 port){
	SDLNet_ResolveHost(&m_ip,nullptr,port);
	m_socket = SDLNet_TCP_Open(&m_ip);
}

TCPServer::~TCPServer(){
	SDLNet_TCP_Close(m_socket);
}

bool TCPServer::Accept(TCPConnection& t){
	auto sock = SDLNet_TCP_Accept(m_socket);
	if(sock == nullptr) return false;
    else{
        t.m_socket = sock;
        return true;
    }
}

void TCPServer::ListenAndAccept(TCPConnection& t){
	while(1){
        if(Accept(t)) return;
		else SDL_Delay(5);
	}
}


