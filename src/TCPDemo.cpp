
#include "Core.h"
#include <iostream>
using namespace std;
using namespace Core;

TCPConnection tcp;

void Send(THREAD_ID id) {
    bool* data = (bool*)GetData(id);

    while(1) {
        cout<<"Send";
        for(int i = 0;i<6;++i) cout<<data[i];
        cout<<endl;
        if(tcp.Send(data, sizeof(bool[8]))<1) {
            return;
        }
        if(data[0]) {
            return;
        }
    }
}

class TCPDemo:public Activity {
private:
    SDL_Rect m_r ={0,0,10,10};
    volatile bool m_b[8] = {false};
    Thread t;
public:
    TCPDemo():t(&Send)
    {};
    void OnShow() {
        t.Run();
        cout<<m_b[0]<<endl;
        t.SendData(&m_b);
    }

    void OnDraw() {
        SDL_SetRenderDrawColor(pRnd, 255, 255, 255, 255);
        SDL_RenderFillRect(pRnd, &m_r);
    };

    void OnNext() {
        SDL_Rect rr;

        if(tcp.Recv(&rr, sizeof(rr)) != 0) {
            m_r = rr;
        }

    }

    void OnEvent(const SDL_Event& e) {
        bool s;

        if(e.type == SDL_KEYDOWN) {
            s = true;
        }

        if(e.type == SDL_KEYUP) {
            s = false;
        }

        std::string b(SDL_GetKeyName(e.key.keysym.sym));
        cout<<b<<endl;

        if(b == "Up") {
            m_b[4] = s;
        }

        if(b == "Right") {
            m_b[1] = s;
        }

        if(b == "Down") {
            m_b[2] = s;
        }

        if(b == "Left") {
            m_b[3] = s;
        }

        if(b == "Left Shift") {
            m_b[5] = s;
        }
    }

    void OnHide() {
        m_b[0] = true;
    }
};

void Main(const std::vector<std::string> args){
    ResVal r;
    r.Load("tcpdemo.rv");
    TCPDemo a;
    tcp.ConnectToServer("127.0.0.1",16384);

    if(!tcp.Connected()) {
        cout<<"fail"<<SDLNet_GetError();
        return;
    }

    pRnd.Create("TCPDemo", 0, 1024, 768);
    CoreRun(&a);
}

