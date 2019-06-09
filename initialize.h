/**
 * @File  initialize.h
 * @Brief 包含结构体Window_Render, MapSurfaceTex, Map,
 *        Newborn_idx, Tank, TankSurfaceTex, Key_to_Num, Timer的定义
 * 对基础信息进行整合定义，并提供函数进行创建
 * @Date  2018/05
 */


#pragma once
#ifndef _INITIALIZE_
#define _INITIALIZE_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>

#define _AIR_   0
#define _WALL_  1
#define _STEEL_ 2
#define _GRASS_ 3
#define _WATER_ 4

#define _NORTH_ 0
#define _EAST_  90
#define _SOUTH_ 180
#define _WEST_  270

#define _P1_MOVE_   0
#define _P2_MOVE_   1
#define _P1_ATTACK_ 2
#define _P2_ATTACK_ 3
#define _PAUSE_     4
#define _ESCAPE_    5
#define _MOVE_PLUS  6

#define _CLOCK_DEFAULT 3

const int bcg_alpha   = 200;
const int delay_time  = 15;
const int block_num   = 26;         // 0 <= x,y < block_num
const int block_len   = 30;
const int infor_len   = 300;
const int operate_num = 6 + 2;      // 2个预备方向

const int player_num    = 2;
const int enemy_num     = 3;
const int player_speed  = 3;
const int enemy_speed   = 2;
const int missile_speed = 5;
const int player_rect_timer = 5;
const double missile_gap    = 0.5;
const int enemy_reborn_gap = 5;
const int player_reborn_gap = 3;

class Window_render;
class Newborn_idx;
class MapSurfaceTex;
class TankSurfaceTex;
class Tank;
class Timer;
class Map;

class Timer {
public:
    Timer(int, int);   // 时间间隔   附加信息   (初始不计)
    Timer(int);        // 时间间隔              (初始不计)
    Timer();           // (初始计数)
    ~Timer();
    void SetClockGap(double);
    void SetRecordClock();
    int GetRecordClock();
    int GetResetTime();
    bool Is_overGap();
    int GetMsg();
private:
    clock_t clock_record;
    clock_t clock_now;
    double clock_gap;
    bool is_pause;
    int reset_time;
    int msg;
};

class Window_render {
public:
	SDL_Window* window;
	SDL_Renderer* rend;
	/*---------param--------fuction----------*/
	Window_render();
	~Window_render();
};

class Newborn_idx {
public:
	class player {
	public:
		player();
		void Get_idx(int, SDL_Rect&)const;
	private:
		std::vector<int> idx_x;
		std::vector<int> idx_y;
	};
	class enemy {
	public:
		enemy();
		void Get_idx(int, SDL_Rect&)const;
	private:
		std::vector<int> idx_x;
		std::vector<int> idx_y;
	};
};

class MapSurfaceTex {
public:
	SDL_Surface* wall;
	SDL_Surface* steel;
	SDL_Surface* water;
	SDL_Surface* grass;
	SDL_Surface* heart;
	SDL_Surface* background;
    SDL_Surface* scoreboard;
	SDL_Texture* wall_tex;
	SDL_Texture* steel_tex;
	SDL_Texture* water_tex;
	SDL_Texture* grass_tex;
	SDL_Texture* heart_tex;
    SDL_Texture* scoreboard_tex;
	SDL_Texture* background_tex;
    SDL_Rect scb_rect;
	SDL_Rect bcg_rect;
	SDL_Rect map_rect;
	/*---------param--------fuction----------*/
	MapSurfaceTex(SDL_Renderer*);
	~MapSurfaceTex();
};

class TankSurfaceTex {
public:
	std::vector<SDL_Surface*>        tank_player;
	std::vector<SDL_Surface*>     tank_enemy_com;
	std::vector<SDL_Texture*>    tank_player_tex;
	std::vector<SDL_Texture*> tank_enemy_com_tex;
	SDL_Surface*     player_missile;
	SDL_Surface*      enemy_missile;
	SDL_Texture* player_missile_tex;
	SDL_Texture*  enemy_missile_tex;
	int tank_player_num;
	int tank_enemy_com_num;
	/*---------param--------fuction----------*/
	TankSurfaceTex(int, SDL_Renderer*);
	~TankSurfaceTex();
};

/*         包含Tank_player, Tank_enemy两个类          */
class Tank {
public:
	/*    包含tank_p1, tank_p2, player_num, rect, point   */
	class Tank_player {
	public:
		std::vector<SDL_Rect>   rect;
		std::vector<SDL_Point> point;
		std::vector<int>   direction;
		std::vector<bool>  invincible;
        std::vector<SDL_Rect>       update_rect;
        std::vector<Timer> player_missile_timer;
        std::vector<Timer> player_reborn;
        int  player_rect_timer;
		int  player_num;
		/*       包含玩家坦克的发射物一个类         */
		class Missile {
		public:
			SDL_Rect rect;
			int direction;
			int belong_num;
            bool be_hide;
			/*---------param--------fuction----------*/
            Missile();
			~Missile();
            void Set_missile(Tank::Tank_player &player, int);
		};
		/*---------param--------fuction----------*/
		Tank_player(int, TankSurfaceTex&);
		~Tank_player();
        void UpDateTankRect(int);              // ??????????????????????
	};
	class Tank_enemy {
	public:
		SDL_Rect   rect;
		SDL_Point point;
		int   direction;
		int  enemy_kind;        //com: 1 ~ 3; spec: > 4;
        int born_idx;
        SDL_Rect update_rect;
        Timer    enemy_missile_timer;
		class Missile {
        public:
			SDL_Rect rect;
			int direction;
			int belong_num;
            bool be_hide;
			/*---------param--------fuction----------*/
            Missile();
			~Missile();
            Missile(int, int, int, int);    //direction  belong_num   rect.x    rect.y
		};
		/*---------param--------fuction----------*/
		Tank_enemy(int, int, TankSurfaceTex &);       //出生在几号位置  坦克的种类
		~Tank_enemy();
	};
};

class Map {
public:
	bool alive;
	int element_kind;          //[0:4] => { air, wall, steel, grass, water }
    /*---------param--------fuction----------*/
	Map(): alive(false), element_kind(0) {}
	~Map() {};
};

class Key_to_Num {
public:
    int key_state[operate_num];
    std::map <SDL_Keycode, int> key_map;
    /*---------param--------fuction----------*/
    Key_to_Num();
    ~Key_to_Num();
    bool SetKeyState(SDL_Event&, Tank::Tank_player&);
};

#endif // !_INITIALIZE_
