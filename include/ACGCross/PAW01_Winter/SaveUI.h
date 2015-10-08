#ifndef _HEAD_SAVEUI_HEAD_
#define _HEAD_SAVEUI_HEAD_

#include "Snow.h"
#include "Button.h"

namespace ACGCross{

class SaveUI : public ::Snow::Activity
{
    public:
        SaveUI();
        void MoveDuang(int mouse_x,int mouse_y);
        void OnShow();
        void OnHide();
        void OnNext();
        void OnDraw();
        void OnEvent(const SDL_Event& e);
        void OnEvent(::Snow::Control*,const Sint32);
        void NeedReturn();
        void SetCallByTitle(bool b){m_callByTitle = b;}
        bool GetCallByTitle(){return m_callByTitle;}
        void DLoad(int);
    protected:
    private:
        int m_dLoad;Uint8 m_dLoad_fg;
        ::Snow::Surface m_bg;
        ::Snow::Sprite m_bgt_o;
        ::Snow::Sprite m_bgt;

        bool m_callByTitle = false;

        ::Snow::Timer m_timer;
        enum {SHOWING,HIDING,NOR,TITLING} m_stat = NOR;

        class SaveButton:public ::Snow::Control{
        private:
            Sint8 m_saveID;
            SDL_Point m_orgPos;
            ::Snow::Sprite m_button;
        public:
            SaveButton(Sint8 saveID):m_saveID(saveID){};
            bool OnEvent(const SDL_Event&,Activity& a);
            void OnDraw();
            void SetPos(int x,int y);
            SDL_Point GetPos();
            void SetPosOffset(int x,int y);
            void SetShowing(float per);
            //void SetHiding(float per);
            void LoadSurface(SDL_Surface* pSur);
            virtual ~SaveButton(){};
        } *m_saves [4][4];

        class Really:public ::Snow::Activity{
        private:
            SDL_Rect m_bg;
            Button m_save;
            Button m_load;
            Button m_del;
            SaveButton* m_saveBtn;
            bool m_needRet = false;
            int m_saveNum;

            enum{SO_MENU,SO_BTN,HI_BTN,HI_MENU,NOR} m_state = NOR;
            ::Snow::Timer m_timer;
            bool m_callByTitle;
        public:
            Really();
            void SetInit(int x,int y,int dataNum,SaveButton*);
            void OnShow();
            void OnHide();
            void OnDraw();
            void OnNext();
            void OnEvent(const SDL_Event&);
            void OnEvent(::Snow::Control*,const Sint32);
        }m_actReally;
};
}

#endif // _HEAD_SAVEUI_HEAD_
