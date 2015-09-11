#include "TCPNet.h"

using namespace Core;

TCPConnection* TCPConnection::ConnectToServer(const std::string host,Uint16 port){
	auto ret = new TCPConnection;
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
	SDLNet_TCP_Close(m_socket);
}

TCPServer::TCPServer(Uint16 port){
	SDLNet_ResolveHost(&m_ip,nullptr,port);
	m_socket = SDLNet_TCP_Open(&m_ip);
}

TCPServer::~TCPServer(){
	SDLNet_TCP_Close(m_socket);
}

TCPConnection* TCPServer::Accept(){
	auto sock = SDLNet_TCP_Accept(m_socket);
	if(sock == nullptr) return nullptr;

	auto ret = new TCPConnection;
	ret -> m_socket = sock;
	return ret;
}

TCPConnection* TCPServer::ListenAndAccept(){
	while(1){
		auto ret = Accept();
		if(ret != nullptr) return ret;
		SDL_Delay(5);
	}
}


