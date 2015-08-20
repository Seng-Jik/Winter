#ifndef _HEAD_SOUND_
#define _HEAD_SOUND_

#include "SDLbase.h"
#include <map>
#include "ResFile.h"

/* 声音类 ，一个声音类代表一个通道，可以同时播放。 */
namespace Core{

    class Sound
    {
    private:
        Core::ResFile m_file;
        static unsigned int m_channelCount;
    protected:
        int m_channel;
        Mix_Chunk* m_pCnk;
        void Free();
    public:
        Sound();
        ~Sound();
        static void Init();	//初始化音频系统
        static void Quit();	//结束音频系统
        bool Load(const std::string&);	//加载声音
        void Play(const int _fadetime = 0);	//播放声音，可循环，可定循环开始点，可淡出淡入
        void Play_Loop(const int _fadetime = 0);	//无循环点循环播放
        void Pause();
        void Resume();
        void Stop(const int _fadetime = 0);
        void Volume(const int _vol);	//音量
        bool Playing(); //是否正在播放
    };

    void Sound_CallBack(const int channel);
    class Sound_Loop:public Sound	//有循环点循环声音类，由两个文件组成，头部音乐和循环部分音乐
    {
    friend void Core::Sound_CallBack(const int channel);
    private:
        Mix_Chunk* m_pCnk_loop;
        Core::ResFile m_loopFile;
        static std::map<int,Mix_Chunk*> m_ChnToCnk;	//频道对循环文件关系
    protected:
        void Free();
    public:
        Sound_Loop();
        ~Sound_Loop();
        bool Load(const std::string&,const std::string&);
        void Play(const int _fadetime = 0);
        void Play_Loop(const int _fadetime = 0);
        void Stop(const int _fadetime = 0);
    };

}

#endif // _HEAD_SOUND_
