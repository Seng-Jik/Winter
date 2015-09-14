#include "Core/Thread.h"
#define THREAD_WAIT 25
using namespace Core;
//子线程内提供的API实现
namespace Core
{
    int LaunchThread(THREAD_ID ThreadObject)	//这个函数已经开始新线程了
    {
        ((Thread*)ThreadObject) -> Lock();	//线程类初始化
        ((Thread*)ThreadObject) -> m_imsg = 0;
        ((Thread*)ThreadObject) -> m_omsg = 0;
        ((Thread*)ThreadObject) -> m_running = true;
        void (*thread)(THREAD_ID) = ((Thread*)ThreadObject) -> m_func;
        ((Thread*)ThreadObject) -> Unlock();

        (*thread)(ThreadObject);	//执行线程

        ((Thread*)ThreadObject) -> Lock();	//线程结束
        ((Thread*)ThreadObject) -> m_imsg = 0;
        ((Thread*)ThreadObject) -> m_running = false;
        ((Thread*)ThreadObject) -> Unlock();
        return 0;
    }

    int GetMsg(THREAD_ID obj)
    {
        ((Thread*)obj) -> Lock();
        int msg = ((Thread*)obj) -> m_imsg;
        ((Thread*)obj) -> m_imsg = 0;
        ((Thread*)obj) -> Unlock();
        return msg;
    }

    void ReturnMsg(THREAD_ID obj,const int msg)
    {
        while(1)
        {
            ((Thread*)obj) -> Lock();
            if (((Thread*)obj) -> m_omsg !=0)
            {
                ((Thread*)obj) -> Unlock();
                SDL_Delay(THREAD_WAIT);
                continue;
            }
            else
            {
                ((Thread*)obj) -> m_omsg = msg;
                ((Thread*)obj) -> Unlock();
                break;
            }
        }
    }

    void WaitMsg(THREAD_ID obj,const int msg)
    {
        while(GetMsg(obj) != msg)
            SDL_Delay(THREAD_WAIT);
    }

    int WaitMsg(THREAD_ID obj)
    {
        int ret;
        while(1)
        {
            ret = GetMsg(obj);
            if(ret != 0) break;
            else SDL_Delay(THREAD_WAIT);
        }
        return ret;
    }

    volatile void* GetData(THREAD_ID obj)
    {
        void* ret;
        while(1)
        {
            ((Thread*)obj) -> Lock();
            if(((Thread*)obj) -> m_idata != nullptr)
            {
                ret = (void*)(((Thread*)obj) -> m_idata);
                ((Thread*)obj) -> m_idata = nullptr;
                ((Thread*)obj) -> Unlock();
                return ret;
            }
            else
            {
                ((Thread*)obj) -> Unlock();
                SDL_Delay(THREAD_WAIT);
            }
        }
    }
    void ReturnData(THREAD_ID obj,volatile void* data)
    {
        while(1)
        {
            ((Thread*)obj) -> Lock();
            if(((Thread*)obj) -> m_odata == nullptr)
            {
                ((Thread*)obj) -> m_odata = data;
                ((Thread*)obj)-> Unlock();
                return;
            }
            else
            {
                ((Thread*)obj) -> Unlock();
                SDL_Delay(THREAD_WAIT);
            }
        }
    }

    //Thread类实现
    Thread::Thread(void (*func)(THREAD_ID))
    {
        m_imsg = 0;
        m_omsg = 0;
        m_running = false;
        m_thread = nullptr;
        m_func = func;
        m_idata = nullptr;
        m_odata = nullptr;
    }

    Thread::~Thread()
    {
        WaitFinish();
    }

    void Thread::Run()
    {
        if (Running()) return;
        if (m_thread != nullptr) WaitFinish();
        m_thread = SDL_CreateThread(&LaunchThread,"Thread",(void*)this);
    }

    void Thread::SendMsg(const int msg)
    {
        while(1)
        {
            Lock();
            if(m_imsg != 0)
            {
                Unlock();
                SDL_Delay(THREAD_WAIT);
                continue;
            }
            else
            {
                m_imsg = msg;
                Unlock();
                break;
            }
        }
    }

    bool Thread::Running()
    {
        Lock();
        bool ret = m_running;
        Unlock();
        return ret;
    }

    void Thread::WaitFinish()
    {
        int i;
        if(Running()) SDL_WaitThread(m_thread,&i);
        m_thread = nullptr;
    }

    int Thread::GetMsg()
    {
        Lock();
        int ret = m_omsg;
        m_omsg = 0;
        Unlock();
        return ret;
    }

    int Thread::WaitMsg()
    {
        int ret;
        while(1)
        {
            ret = GetMsg();
            if(ret != 0) break;
            else SDL_Delay(THREAD_WAIT);
        }
        return ret;
    }

    void Thread::WaitMsg(const int msg)
    {
        while(GetMsg() != msg) SDL_Delay(THREAD_WAIT);
    }

    void Thread::SendData(volatile void* data)
    {
        while(1)
        {
            Lock();
            if(m_idata == nullptr)
            {
                m_idata = data;
                Unlock();
                return;
            }
            else
            {
                Unlock();
                SDL_Delay(THREAD_WAIT);
            }
        }
    }

    volatile void* Thread::GetData()
    {
        void* ret;
        while(1)
        {
            Lock();
            if(m_odata != nullptr)
            {
                ret = (void*)m_odata;
                m_odata = nullptr;
                Unlock();
                return ret;
            }
            else
            {
                Unlock();
                SDL_Delay(THREAD_WAIT);
            }
        }
    }

}
