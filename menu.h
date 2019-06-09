/**
 * @File  menu.h
 * @Brief 鐢ㄦ潵瀹氫箟鑿滃崟鐣岄潰绫诲拰鍑芥暟
 * @Date  2018-05-20
 */

#pragma once
#ifndef  _MENU_
#define  _MENU_

#pragma comment (lib,"Urlmon.lib")
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>

#define _MENU_BCG_     0
#define _MENU_EDGE_    1
#define _MENU_TITLE_   2
#define _MENU_DIRECT_  3
#define _MENU_PRA_MOD_ 4
#define _MENU_ADV_MOD_ 5
#define _MENU_CIRCLE_  6
#define _MENU_BACK_    7
#define _MENU_ENTER_   8
#define _MENU_MOUSE_   9

#define _OP_NULL_    0
#define _OP_DIRECT_  1
#define _OP_PRA_MOD_ 2
#define _OP_ADV_MOD_ 3

#define _IS_DIRECT_  (event.button.x>=conmon_x&&event.button.x<=conmon_w+conmon_x&&event.button.y>=direct_y&&event.button.y<=direct_y+conmon_h)
#define _IS_PRA_MOD_ (event.button.x>=conmon_x&&event.button.x<=conmon_w+conmon_x&&event.button.y>=pra_mod_y&&event.button.y<=pra_mod_y+conmon_h)
#define _IS_ADV_MOD_ (event.button.x>=conmon_x&&event.button.x<=conmon_w+conmon_x&&event.button.y>=adv_mod_y&&event.button.y<=adv_mod_y+conmon_h)
#define _IS_BACK_    (event.button.x>=back_x&&event.button.x<=back_w+back_x&&event.button.y>=back_y&&event.button.y<=back_y+back_h)
#define _IS_ENTER_   (event.button.x>=back_x&&event.button.x<=back_w+back_x&&event.button.y>=enter_y&&event.button.y<=enter_y+back_h)

#define _ALPHA_TOTAL_ 255


const int font_size = 1000;

const int delay_menu_time = 15;
const int window_size_x = 800;
const int window_size_y = 800;

/*            元锟斤拷位锟矫诧拷锟斤拷             */
const int title_x   = 160;
const int title_y   = 50;
const int title_w   = 480;
const int title_h   = 130;
const int conmon_x  = 230;
const int conmon_w  = 320;
const int conmon_h  = 90;
const int direct_y  = 245;
const int pra_mod_y = 375;
const int adv_mod_y = 500;
const int init_o_x  = 190;
const int init_o_y  = 220;
const int init_o_w  = 390;
const int init_o_h  = 130;
const int back_x    = 310;
const int back_y    = 80;
const int back_w    = 140;
const int back_h    = 60;
const int enter_y   = 670;
const int mouse_w   = 30;
const int mouse_h   = 30;

const int motion_wave = 10;

class Menu_Render;
class Menu_SurTex;
class GameMode;

void DrawMenu(Menu_Render &, Menu_SurTex &);
bool GamemodeCal(GameMode &, Menu_Render &, Menu_SurTex &);
void DirectMod(SDL_Renderer *, Menu_SurTex &);
bool Pra_Mod(GameMode &, SDL_Renderer *, Menu_SurTex &);
bool Adv_Mod(GameMode &, SDL_Renderer *, Menu_SurTex &);
GameMode MenuWorking();


/******************* 锟斤拷锟斤拷锟斤拷 ***********************/

class Menu_Render {
public:
    Menu_Render();
    ~Menu_Render();
    SDL_Window *GetWindow();
    SDL_Renderer *GetRender();
private:
    SDL_Window *window;
    SDL_Renderer *rend;
};

class Menu_SurTex {
public:
    Menu_SurTex(SDL_Renderer *);
    ~Menu_SurTex();
    SDL_Surface *GetSurface(int);
    SDL_Texture *GetTexture(int);
    SDL_Rect     GetRect(int);
    void SetMotion_OP(SDL_Event &);
    bool SetMouseDown_OP(SDL_Event &, GameMode &);
    bool SetKeyDown_OP(SDL_Event &, GameMode &);
    bool CmpCon_op(int);
    void DrawCircle(SDL_Renderer *, int);
    bool DrawStep(SDL_Renderer *, SDL_Event &, int);
    void ResetOption() { option = con_op = 0; }
    void ResetMouseRect(SDL_Event &);
private:
    std::vector<SDL_Surface*> MenuSur_list;
    std::vector<SDL_Texture*> MenuTex_list;
    std::vector<SDL_Rect>     MenuRect_list;
    int option;
    int con_op;
};

class GameMode {
public:
    GameMode() : map_order(10), mode(1), player_num(2) {};
    ~GameMode() { /* DO NOTHING */ };
    int GetMode() { return this -> mode; }
    int GetMap_Order() { return this -> map_order; }
    int GetPlayer_num() { return this -> player_num; }
    void SetMode(int mode) { this -> mode = mode; }
    void SetMap_order(int Map_order) { this -> map_order = Map_order; }
    void SetPlayer_num(int player) { this -> player_num = player; }
private:
    int map_order;
    int mode;
    int player_num;
};

#endif // _MENU_




