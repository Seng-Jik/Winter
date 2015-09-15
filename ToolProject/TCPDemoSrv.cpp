#include "SDL_net.h"
#include "SDL.h"
#include <cstdio>
#include <iostream>
using namespace std;

int main(int argc,char** argv) {
    SDL_Init(SDL_INIT_TIMER);
    SDLNet_Init();
    IPaddress ip;
    SDL_Rect poi = {500, 500,10,10};

    if(SDLNet_ResolveHost(&ip, nullptr, 16384) != 0) {
        printf("Resolve Fail\n");
    }

    TCPsocket tcp = SDLNet_TCP_Open(&ip);

    if(tcp == nullptr) {
        printf("Ser Fail\n");
    }

    while(1) {

        printf("Listing...\n");
        TCPsocket client = nullptr;

        while(1) {
            client = SDLNet_TCP_Accept(tcp);

            if(client != nullptr) {
                break;
            }

            SDL_Delay(20);
        }

        printf("Link Start!\n");


        Uint32 jsq = 0;
        while(1) {
            bool bufIn [8];

            if(SDLNet_TCP_Recv(client, &bufIn, sizeof(bufIn)) < 0) {
                SDL_Delay(1);\
                jsq ++;
                if(jsq >= 5000) break;
                continue;
            }

            jsq = 0;
            //cout<<"Move";
            //for(int i = 0;i<6;++i) cout<<(int)bufIn[i];
            int speed = 10;

            if(bufIn[5]) {
                speed = 5;
            }

            if(bufIn[1] && poi.x < 1004) {
                poi.x += speed;
            }

            if(bufIn[2] && poi.y < 748) {
                poi.y += speed;
            }

            if(bufIn[3] && poi.x > 20) {
                poi.x -= speed;
            }

            if(bufIn[4] && poi.y > 20) {
                poi.y -= speed;
            }

            //cout<<","<<poi.x<<","<<poi.y<<endl;

            if(SDLNet_TCP_Send(client, &poi, sizeof(poi)) < 1) {
                SDLNet_TCP_Close(client);
                printf("Service Stopped.1\n");
                break;
            }

            if(bufIn[0]) {
                SDLNet_TCP_Close(client);
                printf("Service Stopped.2\n");
                break;
            }
        }
    }

    return 0;
}

