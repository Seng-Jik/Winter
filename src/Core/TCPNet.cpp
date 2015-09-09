#include "TCPNet.h"

using namespace Core;

TCPSocket* TCPSocket::LinkToServer(const std::string host,Uint16 port){
	auto ret = new TCPSocket;
	if(SDLNet_ResolveHost(&(ret -> m_ip),host.c_str(),port) == -1){
		delete ret;
		return nullptr;
	}
	ret -> m_socket = SDLNet_TCP_Open(&(ret -> m_ip));
	if(ret -> m_socket == nullptr){
		delete ret;
		return nullptr;
	}
	return ret;
}

int TCPSocket::Send(const void* m,int size){
	return SDLNet_TCP_Send(m_socket,m,size);
}

int TCPSocket::Recv(void* m,int size){
	return SDLNet_TCP_Recv(m_socket,m,size);
}


int TCPSocket::WaitRecv(void* m,int size){
	int ret;
	while(1){
		ret = Recv(m,size);
		if(ret > 0) return ret;
		SDL_Delay(20);
	}
}

TCPSocket::~TCPSocket(){
	SDLNet_TCP_Close(m_socket);
}

TCPServer::TCPServer(Uint16 port){
	SDLNet_ResolveHost(&m_ip,nullptr,port);
	m_socket = SDLNet_TCP_Open(&m_ip);
}

TCPServer::~TCPServer(){
	SDLNet_TCP_Close(m_socket);
}

TCPSocket* TCPServer::Accept(){
	auto sock = SDLNet_TCP_Accept(m_socket);
	if(sock == nullptr) return nullptr;
	
	auto ret = new TCPSocket;
	ret -> m_socket = sock;
	return ret;
}

TCPSocket* TCPServer::ListenAndAccept(){
	while(1){
		auto ret = Accept();
		if(ret != nullptr) return ret;
		SDL_Delay(5);
	}
}


