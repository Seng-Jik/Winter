#include "Snow.h"
#include <cstdlib>
#include <ctime>
#include "ACGCross/DASPReader.h"
#include "ACGCross/PAW01_Winter/Game.h"
#include "ACGCross/ACGCross_Logo.h"
using namespace Snow;
using namespace ACGCross;
int main(int argc,char** argv)
{
    Init();

    /*DASPReader dasp;
    ifstream inDic("dic.dasdic",ios::binary);
    char dic[4096];inDic.read(dic,4096);
    dasp.SetDic(dic);
    dasp.OpenDASP("out.dasp");
    ResFile::InstallReader(&dasp);*/

    struct screen{int w,h;bool full;};
    srand(time(NULL));
    screen screenmode[] ={
        {1024,768,false},
        {1280,960,false},
        {1024,768,true},
        {1280,960,true},
        {-1,-1,true},
        {1024,576,false},
        {1280,720,false},
        {1024,576,true},
        {1280,720,true},
        {-1,-1,true}
    };
    pRnd.Create("冬",screenmode[gameData.GetGrpMode()].full,
            screenmode[gameData.GetGrpMode()].w,screenmode[gameData.GetGrpMode()].h);
    InitGame();

    #ifdef _DEBUG
    Run(pGal);   //启动Galgame活动
    #else

    ACGCross::Logo l;
    l.SetGoto(*pTitle);
    Run(&l);
    #endif // _DEBUG
    exit(0);
}
