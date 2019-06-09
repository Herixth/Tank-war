/**
 * @File  initialize.cpp
 * @Brief 用来进行initialize.h中各个类函数的具体实现
 * @Date  2018/05
 */

#include "initialize.h"

#include <vector>
#include <iostream>

/********************* class: Window_render ************************/
Window_render::Window_render() {
	window = SDL_CreateWindow
	("Tank War", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      block_num * block_len + infor_len, block_num * block_len, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(rend);
}

Window_render::~Window_render() {
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
}
/*******************************************************************/

/********************* class: Newborn_idx **************************/
    /*                 class: player                     */
    Newborn_idx::player::player() {
	    idx_x.push_back(8  * block_len);
	    idx_x.push_back(16 * block_len);
	    idx_y.push_back(24 * block_len);
	    idx_y.push_back(24 * block_len);
    }
	/**
	 * @param num 第几个位置 num∈[1,2]
	 * @param rect 直接引用取值
	 */
	void Newborn_idx::player::Get_idx(int num, SDL_Rect& rect) const {
		auto tmp_x = idx_x.begin() + num - 1;
		auto tmp_y = idx_y.begin() + num - 1;
		rect.x = *tmp_x;
		rect.y = *tmp_y;
	}

    /*                  class: enemy                      */
	Newborn_idx::enemy::enemy() {
		idx_x.push_back(0);
		idx_x.push_back(12 * block_len);
		idx_x.push_back(24 * block_len);
		idx_y.push_back(0);
		idx_y.push_back(0);
		idx_y.push_back(0);
	}
	void Newborn_idx::enemy::Get_idx(int num, SDL_Rect& rect)const {
		auto tmp_x = idx_x.begin() + num - 1;
		auto tmp_y = idx_y.begin() + num - 1;
		rect.x = *tmp_x;
		rect.y = *tmp_y;
	}
/*******************************************************************/

/********************** class: MapSurfaceTex ***********************/
MapSurfaceTex::MapSurfaceTex(SDL_Renderer* rend) {
	wall  = IMG_Load("image/map/wall.gif");
	steel = IMG_Load("image/map/steel.gif");
	water = IMG_Load("image/map/water.gif");
	grass = IMG_Load("image/map/grass.png");
	heart = IMG_Load("image/map/heart.png");
    scoreboard = IMG_Load("image/map/scoreboard.png");
	background = IMG_Load("image/map/background.png");
	wall_tex  = SDL_CreateTextureFromSurface(rend, wall);
	steel_tex = SDL_CreateTextureFromSurface(rend, steel);
	water_tex = SDL_CreateTextureFromSurface(rend, water);
	grass_tex = SDL_CreateTextureFromSurface(rend, grass);
	heart_tex = SDL_CreateTextureFromSurface(rend, heart);
    scoreboard_tex = SDL_CreateTextureFromSurface(rend, scoreboard);
	background_tex = SDL_CreateTextureFromSurface(rend, background);
	map_rect.x = map_rect.y = bcg_rect.x = bcg_rect.y = 0;
	map_rect.h = map_rect.w = block_len;
	bcg_rect.h = bcg_rect.w = block_len * block_num;
    scb_rect.x = block_len * block_num;
    scb_rect.y = 0;
    scb_rect.h = block_len * block_num;
    scb_rect.w = infor_len;
    SDL_SetTextureBlendMode(background_tex, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(background_tex, bcg_alpha);
}

MapSurfaceTex::~MapSurfaceTex() {
	SDL_FreeSurface(wall);
	SDL_FreeSurface(steel);
	SDL_FreeSurface(water);
	SDL_FreeSurface(grass);
	SDL_FreeSurface(heart);
    SDL_FreeSurface(scoreboard);
	SDL_FreeSurface(background);
	SDL_DestroyTexture(wall_tex);
	SDL_DestroyTexture(steel_tex);
	SDL_DestroyTexture(water_tex);
	SDL_DestroyTexture(grass_tex);
	SDL_DestroyTexture(heart_tex);
    SDL_DestroyTexture(scoreboard_tex);
	SDL_DestroyTexture(background_tex);

}
/*******************************************************************/

/********************** class: Tank ********************************/
    /*              class: Tank_player            */
    Tank::Tank_player::Tank_player(int player_num, TankSurfaceTex &tank_sur_tex) {
		this -> player_num = player_num;
        this -> player_rect_timer = 0;
		rect.resize(player_num);
		point.resize(player_num);
		direction.resize(player_num);
		invincible.resize(player_num);
        update_rect.resize(player_num);
        player_missile_timer.resize(player_num);
        player_reborn.resize(player_num);
		Newborn_idx::player* player = new Newborn_idx::player;

		for (int inc = 0; inc < player_num; inc ++) {
			player -> Get_idx(inc + 1, rect[inc]);
            update_rect[inc].h = update_rect[inc].w = (*(tank_sur_tex.tank_player.begin() + inc)) -> w;
            update_rect[inc].x = update_rect[inc].y = 0;
			rect[inc].w = 2 * block_len;
			rect[inc].h = 2 * block_len;
			point[inc].x = block_len;
			point[inc].y = block_len;
			direction[inc]  = 0;
			invincible[inc] = false;
            player_missile_timer[inc].SetClockGap(missile_gap);
            player_reborn[inc].SetClockGap(player_reborn_gap);
		}
		delete player;
    }
	Tank::Tank_player::~Tank_player() {
		/* DO NOTHING*/
	}

    /* Very nubility function... At least god understands */
    void Tank::Tank_player::UpDateTankRect(int player_order) {
        if (!((this -> player_rect_timer + 1) % ::player_rect_timer)) {
            update_rect[player_order - 1].y = (update_rect[player_order - 1].y + update_rect[player_order - 1].w) % (3 * update_rect[player_order - 1].w);
        }
        this -> player_rect_timer = (this -> player_rect_timer + 1) % ::player_rect_timer;
    }

	    /*          class: Missile           */
        Tank::Tank_player::Missile::Missile() {
            this->belong_num = 0;
            this->direction = 0;
            this->be_hide = false;
        }
		Tank::Tank_player::Missile::~Missile() {
			/*DO NOTHING*/
		}
        void Tank::Tank_player::Missile::Set_missile(Tank::Tank_player& player, int player_num) {
            this->belong_num = player_num;
            this->direction = *(player.direction.begin() + player_num - 1);
            this->rect.h = block_len / 2;
            this->rect.w = block_len / 2;
            this->rect.x = (int)((*(player.rect.begin() + player_num - 1)).x +
                (direction == _NORTH_ || direction == _SOUTH_) * 0.75 * block_len + (direction == _EAST_) * 2 * block_len);
            this->rect.y = (int)((*(player.rect.begin() + player_num - 1)).y +
                (direction == _EAST_ || direction == _WEST_) * 0.75 * block_len + (direction == _SOUTH_) * 2 * block_len);
        }
	/*              class: Tank_enemy             */
	Tank::Tank_enemy::Tank_enemy(int idx, int enemy_kind, TankSurfaceTex &tank_sur_tex) {
        this -> born_idx = idx;
		this -> enemy_kind = enemy_kind;
		direction = _SOUTH_;
		Newborn_idx::enemy* enemy = new Newborn_idx::enemy;
		enemy -> Get_idx(idx, rect);
		point.x = block_len;
		point.y = block_len;
		rect.h = 2 * block_len;
		rect.w = 2 * block_len;
        update_rect.x = update_rect.y = 0;
        update_rect.h = (*tank_sur_tex.tank_enemy_com.begin()) -> h;
        update_rect.w = (*tank_sur_tex.tank_enemy_com.begin()) -> w;
		delete enemy;
	}
	Tank::Tank_enemy::~Tank_enemy() {
		/*DO NOTHING*/
	}
	    /*           class: Missile          */
    	Tank::Tank_enemy::Missile::Missile(int direction, int belong_num, int x, int y) {
			this -> belong_num = belong_num;
			this -> direction  = direction;
            this -> rect.x = (int)(x + (direction == _NORTH_ || direction == _SOUTH_) * block_len * 0.75);
            this -> rect.y = y + (direction == _EAST_ || direction == _WEST_) * block_len;
            this -> rect.w = block_len / 2;
            this -> rect.h = block_len / 2;
            this -> be_hide = false;
		}
        Tank::Tank_enemy::Missile::Missile() {
            this -> belong_num = 0;
            this -> direction  = 0;
        }
		Tank::Tank_enemy::Missile::~Missile() {
			/*DO NOTHING*/
		}
/*******************************************************************/

TankSurfaceTex::TankSurfaceTex(int player_num, SDL_Renderer* rend) {
	const char path_player[2][30] = { "image/tank/tank_P1.png", "image/tank/tank_P2.png" };
	const char path_enemy[3][30] = { "image/tank/enemy1.png", "image/tank/enemy2.gif", "image/tank/enemy3.gif" };
	player_missile = IMG_Load("image/tank/missile/tankmissile.gif");
	enemy_missile  = IMG_Load("image/tank/missile/enemymissile.gif");
	player_missile_tex = SDL_CreateTextureFromSurface(rend, player_missile);
	enemy_missile_tex  = SDL_CreateTextureFromSurface(rend, enemy_missile);
	tank_player_num = player_num;
	tank_enemy_com_num = 3;
	tank_player.resize(player_num);
	tank_enemy_com.resize(tank_enemy_com_num);
	tank_player_tex.resize(player_num);
	tank_enemy_com_tex.resize(tank_enemy_com_num);
	for (int inc = 0; inc < tank_enemy_com_num; inc++) {
		if (inc < player_num) {
			tank_player[inc] = IMG_Load(path_player[inc]);
			tank_player_tex[inc] = SDL_CreateTextureFromSurface(rend, tank_player[inc]);
		}
		tank_enemy_com[inc] = IMG_Load(path_enemy[inc]);
		tank_enemy_com_tex[inc] = SDL_CreateTextureFromSurface(rend, tank_enemy_com[inc]);
	}
}

TankSurfaceTex::~TankSurfaceTex() {
	SDL_FreeSurface(player_missile);
	SDL_FreeSurface(enemy_missile);
	SDL_DestroyTexture(player_missile_tex);
	SDL_DestroyTexture(enemy_missile_tex);
	for (int inc = 0; inc < tank_enemy_com_num; inc++) {
		if (inc < tank_player_num) {
			SDL_FreeSurface(tank_player[inc]);
			SDL_DestroyTexture(tank_player_tex[inc]);
		}
		SDL_FreeSurface(tank_enemy_com[inc]);
		SDL_DestroyTexture(tank_enemy_com_tex[inc]);
	}
}

/*******************************************************************/

Key_to_Num::Key_to_Num() {
    memset(key_state, 0, sizeof(key_state));
    /*   1-4: P1 上右下左;    5-8: P2 上右下左;     //  1 - 2 // 0 - 1
     *   9: P1 攻击;    10: P2 攻击;      11: 暂停;     12: 选项 // 3 - 6 // 2 - 5
     */
    key_map[SDLK_w]      = 1;
    key_map[SDLK_d]      = 2;
    key_map[SDLK_s]      = 3;
    key_map[SDLK_a]      = 4;
    key_map[SDLK_UP]     = 5;
    key_map[SDLK_RIGHT]  = 6;
    key_map[SDLK_DOWN]   = 7;
    key_map[SDLK_LEFT]   = 8;
    key_map[SDLK_j]      = 9;
    key_map[SDLK_DELETE] = 10;
    key_map[SDLK_p]      = 11;
    key_map[SDLK_ESCAPE] = 12;
    key_map[SDLK_SPACE]  = 9;
}

Key_to_Num::~Key_to_Num() {
    /*DO NOTHING*/
}

bool Key_to_Num::SetKeyState(SDL_Event &event, Tank::Tank_player& player) {
    switch (event.type) {
    case SDL_QUIT:
        return false;
        break;
    case SDL_KEYDOWN:
        if (key_map[event.key.keysym.sym]) {
            if (key_map[event.key.keysym.sym] <= 8) {
                if (key_state[key_map[event.key.keysym.sym] / 5]) {
                    if (key_state[key_map[event.key.keysym.sym] / 5] != key_map[event.key.keysym.sym])
                        key_state[key_map[event.key.keysym.sym] / 5 + _MOVE_PLUS] = key_map[event.key.keysym.sym];
                }
                else {
                    key_state[key_map[event.key.keysym.sym] / 5] = key_map[event.key.keysym.sym];
                }
            }
            else {
                switch (key_map[event.key.keysym.sym] - 7) {
                case _P1_ATTACK_:
                    if ((*player.player_missile_timer.begin()).Is_overGap()) {
                        key_state[_P1_ATTACK_] = true;
                        (*player.player_missile_timer.begin()).SetRecordClock();
                    }
                    else {
                        key_state[_P1_ATTACK_] = false;
                    }
                    break;
                case _P2_ATTACK_:
                    if ((*(player.player_missile_timer.begin() + 1)).Is_overGap()) {
                        key_state[_P2_ATTACK_] = true;
                        (*(player.player_missile_timer.begin() + 1)).SetRecordClock();
                    }
                    else {
                        key_state[_P2_ATTACK_] = false;
                    }
                    break;
                default:
                    key_state[key_map[event.key.keysym.sym] - 7] = true;
                    break;
                }
            }
        }
        break;
    case SDL_KEYUP:
        if (key_map[event.key.keysym.sym]) {
            if (key_map[event.key.keysym.sym] <= 8) {
                if (key_state[key_map[event.key.keysym.sym] / 5] == key_map[event.key.keysym.sym]) {
                    key_state[key_map[event.key.keysym.sym] / 5] = key_state[key_map[event.key.keysym.sym] / 5 + _MOVE_PLUS];
                    key_state[key_map[event.key.keysym.sym] / 5 + _MOVE_PLUS] = false;
                }
                else {
                    key_state[key_map[event.key.keysym.sym] / 5 + _MOVE_PLUS] = false;
                }
            }
            else {
                key_state[key_map[event.key.keysym.sym] - 7] = false;
            }
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        if ((*(player.player_missile_timer.begin() + 1)).Is_overGap()) {
            key_state[_P2_ATTACK_] = true;
            (*(player.player_missile_timer.begin() + 1)).SetRecordClock();
        }
        else {
            key_state[_P2_ATTACK_] = false;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        key_state[_P2_ATTACK_] = false;
        break;
    default:
        return true;
        break;
    }

    return true;
}

/*******************************************************************/

Timer::Timer() {
    reset_time = 0;
    this -> clock_gap = _CLOCK_DEFAULT;
    clock_now = clock();
    clock_record = clock() - (clock_t)(clock_gap * CLOCKS_PER_SEC);
    is_pause = false;
}

Timer::Timer(int clock_gap) {
    this -> clock_gap = clock_gap;
    clock_now = clock();
    clock_record = clock();
    is_pause = false;
}

Timer::Timer(int clock_gap, int msg) {
    this -> clock_gap = clock_gap;
    clock_now = clock();
    clock_record = clock();
    is_pause = false;
    this -> msg = msg;
}

Timer::~Timer() {
    /*DO NOTHING*/
}

void Timer::SetClockGap(double clock_gap) {
    this -> clock_gap = clock_gap;
}

void Timer::SetRecordClock() {
    reset_time ++;
    clock_record = clock();
}

int Timer::GetRecordClock() {
    return clock_record;
}

int Timer::GetResetTime() {
    return reset_time;
}

int Timer::GetMsg() {
    return msg;
}

bool Timer::Is_overGap() {
    clock_now = clock();
    return (double)(clock_now - clock_record) > (double)clock_gap * CLOCKS_PER_SEC;
}
