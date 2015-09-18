#include "ACGCross/PAW01_Winter/GalgameActivity/ChrMgr.h"
#include "ACGCross/PAW01_Winter/Game.h"
#include "ACGCross/PAW01_Winter/GalgameActivity/GalgameActivity.h"

namespace ACGCross{
    namespace Galgame{

ChrMgr::ChrMgr(){
}

ChrMgr::~ChrMgr(){  //release the memory and clear the map
    for(m_it = m_chr.begin();m_it != m_chr.end();){
        delete m_it->second;
        m_chr.erase(m_it++);
    }
}

bool ChrMgr::DetectChr(int p_index,bool p_del){
    for(m_it = m_chr.begin();m_it != m_chr.end();)
        if(p_index == m_it->first){
            if(p_del){
                delete m_it->second;
                m_chr.erase(m_it++);
            }else{
                m_it++;
            }
            return true;
        }else{
            m_it++;
        }
    Error(0x30005000,"The index of the chr is not exist.");
    return false;
}

void ChrMgr::OnNext(){
    for(m_it = m_chr.begin();m_it != m_chr.end();m_it++)
        m_it->second->OnNext();
}

void ChrMgr::OnDraw(){
    for(m_it = m_chr.begin();m_it != m_chr.end();m_it++)
        m_it->second->OnDraw();
}

void ChrMgr::SMEProc(SMI::SMEvent* pSme){
    if(pSme -> cmd == L"chr"){
        if(pSme->argv.size() == 5){
            m_index = _wtoi(pSme -> argv[0].c_str());
            m_chr[m_index] = new Chr;
            m_chr[m_index]->Load("GalRes/chr/" + WStringToString(pSme -> argv[1]) + ".png",_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),(float)1.0,_wtoi(pSme -> argv[4].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            m_chr[m_index] = new Chr;
            m_chr[m_index]->Load("GalRes/chr/" + WStringToString(pSme -> argv[1]) + ".png",_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),(float)1.0,255);
        }
    }else if(pSme -> cmd == L"achr_m"){
        if(pSme->argv.size() == 8){
            m_index = _wtoi(pSme -> argv[0].c_str());
            m_chr[m_index] = new Chr;
            m_chr[m_index]->Load("GalRes/chr/" + WStringToString(pSme -> argv[1]) + ".png",_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),_wtoi(pSme -> argv[4].c_str()));
            m_chr[m_index]->Move(_wtoi(pSme -> argv[5].c_str()),_wtoi(pSme -> argv[6].c_str()),_wtoi(pSme -> argv[7].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            m_chr[m_index] = new Chr;
            m_chr[m_index]->Load("GalRes/chr/" + WStringToString(pSme -> argv[1]) + ".png",_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),255);
            m_chr[m_index]->Move(_wtoi(pSme -> argv[4].c_str()),_wtoi(pSme -> argv[5].c_str()),_wtoi(pSme -> argv[6].c_str()));
        }
    }else if(pSme-> cmd == L"chr_m2" ){
        if(pSme->argv.size() == 4){
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Move_rt(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Move_rt(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),0);
        }
    }else if(pSme -> cmd == L"achr_ma" ){
        if(pSme->argv.size() == 10){
            m_index = _wtoi(pSme -> argv[0].c_str());
            m_chr[m_index] = new Chr;
            m_chr[m_index]->Load("GalRes/chr/" + WStringToString(pSme -> argv[1]) + ".png",_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),_wtoi(pSme -> argv[4].c_str()));
            m_chr[m_index]->Move(_wtoi(pSme -> argv[5].c_str()),_wtoi(pSme -> argv[6].c_str()),_wtoi(pSme -> argv[7].c_str()));
            m_chr[m_index]->Alpha(_wtoi(pSme -> argv[8].c_str()),_wtoi(pSme -> argv[9].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            m_chr[m_index] = new Chr;
            m_chr[m_index]->Load("GalRes/chr/" + WStringToString(pSme -> argv[1]) + ".png",_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()),255);
            m_chr[m_index]->Move(_wtoi(pSme -> argv[4].c_str()),_wtoi(pSme -> argv[5].c_str()),_wtoi(pSme -> argv[6].c_str()));
            m_chr[m_index]->Alpha(_wtoi(pSme -> argv[7].c_str()),_wtoi(pSme -> argv[8].c_str()));
        }
    }else if(pSme -> cmd == L"unchr"){   //delete the chr
        DetectChr(_wtoi(pSme -> argv[0].c_str()),true);
    }else if(pSme -> cmd == L"chr_m" ){
        if(pSme->argv.size() == 4){
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Move(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Move(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),0);
        }
    }else if(pSme -> cmd == L"chr_a" ){
        if(pSme->argv.size() == 3){
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Alpha(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Alpha(_wtoi(pSme -> argv[1].c_str()),0);
        }
    }else if(pSme -> cmd == L"chr_e" ){  //remained problem
        m_index = _wtoi(pSme -> argv[0].c_str());
        if(DetectChr(m_index,false))
            std::cout << "CMD:echr" << std::endl;
    }else if(pSme -> cmd == L"chr_z" ){
        if(pSme->argv.size() == 3){
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Zoom((float)_wtof(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Zoom((float)_wtof(pSme -> argv[1].c_str()),0);
        }
    }else if(pSme-> cmd == L"chr_x" ){
        m_index = _wtoi(pSme -> argv[0].c_str());
        if(DetectChr(m_index,false)){
            Chr * TmpChr = new Chr;
            TmpChr = m_chr[m_index];
            m_chr[m_index] = m_chr[_wtoi(pSme -> argv[1].c_str())];
            m_chr[_wtoi(pSme -> argv[1].c_str())] = TmpChr;
        }
    }else if(pSme-> cmd == L"chr_zxy" ){
        if(pSme->argv.size() == 4){
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Zoom(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),_wtoi(pSme -> argv[3].c_str()));
        }else{
            m_index = _wtoi(pSme -> argv[0].c_str());
            if(DetectChr(m_index,false))
                m_chr[m_index]->Zoom(_wtoi(pSme -> argv[1].c_str()),_wtoi(pSme -> argv[2].c_str()),0);
        }
    }
}

void ChrMgr::SMEFastProc(SMI::SMEvent* pSme){
    for(m_it = m_chr.begin();m_it != m_chr.end();m_it++)
        m_it->second->FastProc();
}

bool ChrMgr::SMEFinished(SMI::SMEvent* pSme){   //remained problem
    bool a_IsFinished = true;
    return a_IsFinished;
}

void ChrMgr::SaveData(GameDataMgr::SAVE& bundle){
    m_count = m_chr.size();
    bundle.Write<int>(m_count);
    for(m_it = m_chr.begin(); m_it != m_chr.end();m_it ++){
        bundle.Write<int>(m_it->first);
        m_it->second->SaveData(bundle);
    }
}

void ChrMgr::LoadData(GameDataMgr::SAVE& bundle){
    bundle.Read<int>(m_count);
    for(int i = 0; i < m_count;i++){
        bundle.Read<int>(m_index);
        m_chr[m_index] = new Chr;
        m_chr[m_index]->LoadData(bundle);
    }
}

bool ChrMgr::ChrMgrFinishAnimation(){
	for(m_it = m_chr.begin();m_it != m_chr.end();m_it++){
        if(!m_it->second->FinishAnimation()) return false;
	}
	return true;
}

void ChrMgr::Reset(){
    for(m_it = m_chr.begin();m_it != m_chr.end();){
        delete m_it->second;
        m_chr.erase(m_it++);
    }
    m_count = 0;
}

   }
}
