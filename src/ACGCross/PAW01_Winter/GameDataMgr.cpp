#include "ACGCross/PAW01_Winter/GameDataMgr.h"
#include <string>

#define OPENSAVEFILE std::ifstream in("PlayerData.dat",ios::binary)

using namespace std;
using namespace ::Snow;
using namespace ACGCross;

GameDataMgr::GameDataMgr()
{
    m_mtx.Lock();
    OPENSAVEFILE;
    if(!in){
        ERROR_MSGBOX("Can't load GameData.dat");
        Exit(-1);
    }

    //游戏数据长度效验
    in.seekg(0,ios::end);
    if(in.tellg() < 8+8+32+65536*16+8192){
        ERROR_MSGBOX("GameData.dat is invaild.");
    }

    //加载基本数据

    //设置
    in.seekg(0,ios::beg);
    Uint8 setting[8];
    in.read((char*)(&setting[0]),8);
    m_cfg_bgmVol = setting[0];
    m_cfg_seVol = setting[1];
    m_cfg_cvVol = setting[2];
    m_cfg_grpMode = setting[3];

    //游戏状态
    //  存档状态
    in.seekg(8,ios::beg);
    for(int i = 0;i < 2;++i){
        Uint8 b = in.get();
        for(int j = 0;j < 8;++j)
            m_dataExist[i*8 + j] = bool((b >> j) % 2);
    }

    //CG状态
    in.seekg(16,ios::beg);
    for(int i = 0;i < 32;++i){
        Uint8 b = in.get();
        for(int j = 0;j < 8;++j)
            m_cgState[i*8 + j] = bool((b >> j) % 2);
    }

    //存档
    for(int i = 0;i < 16;++i)
        m_save[i].ReadFromFile(in,16+32+i*65536);
        
    //Kidoku
    Uint8 kidokuBuf[8192];
    in.read((char*)kidokuBuf,8192);
    for(int i = 0;i < 8192;++i){
        Uint8 fliter = 0x80;
        for(int j = 0;j < 8;++j){
            kidoku[i*8+j] = kidokuBuf[i] & fliter;
            fliter>>=1;
        }
    }

    //Kidoku
    in.seekg(1048624);
    Uint8 kidokuBuf[8192];
    in.read((char*)kidokuBuf,8192);
    for(int i = 0;i < 8192;++i){
        Uint8 fliter = 0x80;
        for(int j = 0;j < 8;++j){
            kidoku[i*8+j] = kidokuBuf[i] & fliter;
            fliter>>=1;
        }
    }

    in.close();

    m_mtx.Unlock();
}

bool GameDataMgr::GetCSaveExist()
{
    Lock();
    bool ret = m_dataExist[0];
    Unlock();
    return ret;
}

bool GameDataMgr::GetSaveExist()
{
    bool ret = false;
    Lock();
    for(int i = 1;i < 16;++i)
        if(m_dataExist[i]){
            ret = true;
            break;
        }
    Unlock();
    return ret;
}

bool GameDataMgr::GetCGExist()
{
    Lock();
    bool ret = false;
    for(int i = 0;i<8*32;++i){
        if(m_cgState[i]) ret = true;
        break;
    }
    Unlock();
    return ret;
}

void GameDataMgr::AddUpdateTask(int num)
{
    Lock();
    m_updateTask.insert(num);
    Unlock();
}

void GameDataMgr::UpdateData()
{
    Lock();
    if(m_updateTask.empty()){
        Unlock();
        return;
    }
    else {
        std::fstream out("PlayerData.dat",ios::out | ios::in | ios::binary);
        FOR_EACH(p,m_updateTask.begin(),m_updateTask.end()){
            int t = *p;
            if(t == -1){
                //CG更新
                out.seekp(16,ios::beg);
                for(int i = 0;i < 32;++i){
                    Uint8 b = 0;
                    for(int j = 0;j < 8;++j){
                        b = b>>1;
                        if(m_cgState[i*8+j]) b^=0x80;
                    }
                    out.put(b);
                }
            }else if(t == -2){
                //系统状态更新
                //存档状态
                out.seekp(8,ios::beg);
                for(int i = 0;i < 2;++i){
                    Uint8 b = 0;
                    for(int j = 0;j < 8;++j){
                        b = b>>1;
                        if(m_dataExist[i*8+j]) b^=0x80;
                    }
                    out.put(b);
                }
            }else if(t == -3){
                out.seekp(0,ios::beg);
                out.put((char)m_cfg_bgmVol);
                out.put((char)m_cfg_seVol);
                out.put((char)m_cfg_cvVol);
                out.put((char)m_cfg_grpMode);
            }else if(t >= 0 && t < 16){
                //存档更新
                m_save[t].WriteToFile(out,16+32+t*65536);
            }
        }
        //Kidoku Update
<<<<<<< HEAD
        out.seekp(1048624);
=======
>>>>>>> 792ab1e2e41c3fa8e79975e240c76ea08523367e
        Uint8 kidokuBuf[8192] = {0};
        for(int i = 0;i < 8192;++i){
            for(int j = 0;j < 8;++j){
                kidokuBuf[i]<<=1;
                kidokuBuf[i]+=kidoku[i*8+j];
            }
        }
        out.write((char*)&kidokuBuf[0],8192);
        out.close();
    }
    m_updateTask.clear();
    Unlock();
}

bool GameDataMgr::CGOpend(int num)
{
    Lock();
    bool b = m_cgState[num];
    Unlock();
    return b;
}

void GameDataMgr::OpenCG(int num)
{
    Lock();
    m_cgState[num] = true;
    m_updateTask.insert(-1);
    Unlock();
}

void GameDataMgr::Lock()
{
    m_mtx.Lock();
}

void GameDataMgr::Unlock()
{
    m_mtx.Unlock();
}

GameDataMgr::SAVE& GameDataMgr::operator[](int i)
{
    return m_save[i];
}

bool GameDataMgr::GetDataExist(int num)
{
    Lock();
    bool ret = m_dataExist[num];
    Unlock();
    return ret;
}

void GameDataMgr::SetDataExist(int num,bool tf)
{
    Lock();
    m_dataExist[num] = tf;
    m_updateTask.insert(-2);
    Unlock();
<<<<<<< HEAD
}
=======
}
>>>>>>> 792ab1e2e41c3fa8e79975e240c76ea08523367e
