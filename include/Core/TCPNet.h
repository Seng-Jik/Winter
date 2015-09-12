#include "SDLBase.h"
#include <string>

namespace Core{

class TCPServer;

class TCPConnection{ //套接字连接
friend class TCPServer;
public:
	bool ConnectToServer(const std::string service,Uint16 port);
    //作为客户端连接到服务器，若成功返回true

    void Close();
    //关闭连接

    bool Connected();
    //是否已经载入链接
    //此函数不保证链接成功

	int Send(const void*,int size);	//发送数据到另一端，返回已发送的数据量
	int Recv(void*,int maxlen);
	//从另一端接收数据，返回已接受的数据

	int WaitRecv(void*,int maxlen);
	//等待数据到达

	~TCPConnection();
private:
	IPaddress m_ip;
	TCPsocket m_socket = nullptr;
};

class TCPServer{    //TCP服务器
public:
	TCPServer(Uint16 port);
	//传入端口
	~TCPServer();
	bool Accept(TCPConnection&);
	//接收一个连接，如果没有连接请求返回nullptr.
	void ListenAndAccept(TCPConnection&);
	//侦听，等待连接接入并自动接收。
private:
	IPaddress m_ip;
	TCPsocket m_socket = nullptr;
};
}
