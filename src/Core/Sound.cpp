
#include "Core/Sound.h"
using namespace Core;

//std::map<int,Mix_Chunk*> Sound_Loop::m_ChnToCnk;
unsigned int Sound::m_channelCount = 1;
std::map<int,Mix_Chunk*> Sound_Loop::m_ChnToCnk;

void Core::Sound_CallBack(const int channel)
{
	if (Sound_Loop::m_ChnToCnk.count(channel) == 0) return;
	else
	{
		Mix_PlayChannel(channel,Sound_Loop::m_ChnToCnk[channel],-1);
		Sound_Loop::m_ChnToCnk.erase(channel);
	};
}

void Sound::Init()
{
	Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
	Mix_OpenAudio(44100,AUDIO_S16SYS,2,8192);
	Mix_ChannelFinished(&Sound_CallBack);
}

void Sound::Quit()
{
	Mix_CloseAudio();
	Mix_Quit();
}

Sound::Sound()
{
	m_pCnk = nullptr;
	m_channel = m_channelCount;
	m_channelCount++;
}

Sound::~Sound()
{
	Free();

}

void Sound::Free()
{
    Stop();
    if (m_pCnk != nullptr) Mix_FreeChunk(m_pCnk);
    m_pCnk = nullptr;
    m_file.Free();
}

bool Sound::Load(const std::string& file)
{
	if (m_pCnk != nullptr) Free();
    m_file.Load(file.c_str());
	m_pCnk = Mix_LoadWAV_RW((SDL_RWops*)m_file,m_file.Size());
	if (m_pCnk == nullptr) return false;
	else return true;
}

void Sound_Loop::Free()
{
    Stop();
    Sound::Free();
    if (m_pCnk_loop != nullptr)
	{
		Mix_FreeChunk(m_pCnk_loop);
		m_pCnk_loop = nullptr;
	}
	m_loopFile.Free();
}

Sound_Loop::Sound_Loop()
{
	m_pCnk_loop = nullptr;
}

Sound_Loop::~Sound_Loop()
{
	Free();
	m_ChnToCnk.erase(m_channel);
}

bool Sound_Loop::Load(const std::string& _headfile,const std::string& _loopFile)
{
	m_loopFile.Load(_loopFile);
	m_pCnk_loop = Mix_LoadWAV_RW((SDL_RWops*)m_loopFile,m_loopFile.Size());
	if (m_pCnk_loop == nullptr) return false;
	else
	{
		if (_headfile.empty()) return true;
		else return Sound::Load(_headfile);
	};
}


void Sound::Play(const int _fadetime)
{
	if (_fadetime <= 0) Mix_PlayChannel(m_channel,m_pCnk,0);
	else Mix_FadeInChannel(m_channel,m_pCnk,0,_fadetime);
}

void Sound::Play_Loop(const int _fadetime)
{
	if (_fadetime <= 0) Mix_PlayChannel(m_channel,m_pCnk,-1);
	else Mix_FadeInChannel(m_channel,m_pCnk,-1,_fadetime);
}

void Sound::Volume(const int _vol)
{Mix_Volume(m_channel,_vol);}

void Sound::Pause()
{
	Mix_Pause(m_channel);
}

void Sound_Loop::Play_Loop(const int _fadetime)
{
	if (_fadetime <= 0) Mix_PlayChannel(m_channel,m_pCnk_loop,0);
	else Mix_FadeInChannel(m_channel,m_pCnk_loop,-1,_fadetime);
}

void Sound_Loop::Play(const int _fadetime)
{
	if (Sound::m_pCnk == nullptr) Play_Loop(_fadetime);
	Sound::Play(_fadetime);
	m_ChnToCnk [m_channel] = m_pCnk_loop;
}

void Sound::Resume()
{
	Mix_Resume(m_channel);
}

void Sound::Stop(const int _fadetime)
{
    if (Playing()){
        if (_fadetime <= 0) Mix_HaltChannel(m_channel);
        else Mix_FadeOutChannel(m_channel,_fadetime);
    }
}

void Sound_Loop::Stop(const int _fadetime)
{
	Sound::Stop(_fadetime);
	m_ChnToCnk.erase(m_channel);
}

bool Sound::Playing()
{
    return (Mix_Playing(m_channel) != 0);
}



//#include <iostream>
//#include "file_reader.h"
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//int main(int argc, char** argv)
//{
//	{
//		SDL_Init(SDL_INIT_EVERYTHING);
//		Sound::Init();
//		File f1("00.ogg");
//		File f2("01.ogg");
//		{
//			Sound_Loop snd;
//			snd.Load(f1.Get(),f1.Size(),f2.Get(),f2.Size());
//			cout<<Mix_GetError();
//			snd.Play(-1);
//			system("pause");
//		};
//		Sound::Quit();
//		SDL_Quit();
//	}
//	_CrtDumpMemoryLeaks();
//	return 0;
//}
