
#include "Core.h"
#include <iostream>
using namespace std;
using namespace Core;

TCPConnection tcp;

void Send(THREAD_ID id) {
    bool* data = (bool*)GetData(id);

    while(1) {
        if(tcp.Send(data, sizeof(bool[8]))<1) {
            Exit(-1);
        }

        if(data[0]) {
            return;
        }
    }
}

class TCPDemo:public Activity {
private:
    SDL_Rect m_r;
    bool m_b[8] = {false};
    Thread t;
public:
    TCPDemo():t(&Send)
    {};
    void OnShow() {
        t.Run();
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

        if(b == "Up") {
            b[4] = s;
        }

        if(b == "Right") {
            b[1] = s;
        }

        if(b == "Down") {
            b[2] = s;
        }

        if(b == "Left") {
            b[3] = s;
        }

        if(b == "Left Shift") {
            b[5] = s;
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
    tcp.ConnectToServer("192.168.1.100",13099);

    if(!tcp.Connected()) {
        cout<<"fail"<<SDLNet_GetError();
        return;
    }

    pRnd.Create("TCPDemo", 0, 1024, 768);
    CoreRun(&a);
}

