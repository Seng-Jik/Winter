#include "SDL_net.h"
#include "SDL.h"
#include <cstdio>

int main(){
SDL_Init(SDL_INIT_TIMER);
SDLNet_Init();
IPaddress ip;

if(SDLNet_ResolveHost(&ip, nullptr,3098)!=0) printf("Resolve Fail\n");
TCPsocket tcp = SDLNet_TCP_Open(&ip);
if(tcp == nullptr) printf("Ser Fail\n");

while(1){

printf("Listing...\n");
TCPsocket client = nullptr;

while(1){
client = SDLNet_TCP_Accept(tcp);
if(client != nullptr) break;
SDL_Delay(20);
}

printf("Link Start!\n");

SDL_Point poi = {500,500};

while(1){
bool bufIn [8];
if(SDLNet_TCP_Recv(client,&bufIn,sizeof(bufIn)) < 1) {
SDL_Delay(1);
continue;
}

int speed = 10;
if(bufIn[5]) speed = 5;
if(bufIn[1]&&poi.x<1004)poi.x+=speed;
if(bufIn[2]&&poi.y<748)poi.y+=speed;
if(bufIn[3]&&poi.x>20)poi.x-=speed;
if(bufIn[4]&&poi.y>20)poi.y-=speed;

if(SDLNet_TCP_Send(client,&poi, sizeof(poi)) < 1){
SDLNet_TCP_Close(client);
print("Service Stopped.");
break;
}

if(bufIn[0]){
SDLNet_TCP_Close(client);
print("Service Stopped.");
break;
}
}
}
return 0;
}
