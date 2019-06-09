/**
 * @File  operate.cpp
 * @Brief 用来对operate.h中函数定义的实现
 * @Date  2018/06
 */

#include "operate.h"


void Draw_Map(Map map_set[block_num][block_num], MapSurfaceTex& map_infor, SDL_Renderer* rend) {
    /*           方便查找            */
    SDL_Texture* map_tmp[4] = { map_infor.wall_tex, map_infor.steel_tex, map_infor.grass_tex, map_infor.water_tex };

    for (int row = 0; row < block_num; row++) {
        for (int col = 0; col < block_num; col++) {
            map_infor.map_rect.y = row * block_len;
            map_infor.map_rect.x = col * block_len;
            if (row == block_num - 2 && col == block_num / 2 - 1) {
                map_infor.map_rect.w *= 2;
                map_infor.map_rect.h *= 2;
                SDL_RenderCopy(rend, map_infor.heart_tex, NULL, &map_infor.map_rect);
                map_infor.map_rect.w /= 2;
                map_infor.map_rect.h /= 2;
            }
            if (!map_set[row][col].alive || !map_set[row][col].element_kind) continue;
            SDL_RenderCopy(rend, map_tmp[map_set[row][col].element_kind - 1], NULL, &map_infor.map_rect);
        }
    }
}


bool Can_Move_Player(Tank::Tank_player &player, int player_order, Map map_set[block_num][block_num], int move_direct, std::vector<Tank::Tank_enemy> &enemy) {
    int pos_x = (*(player.rect.begin() + player_order - 1)).x;
    int pos_y = (*(player.rect.begin() + player_order - 1)).y;
    int direct = move_direct * 90;
    pos_x += ((direct == 90) - (direct == 270)) * player_speed;
    pos_y += ((direct == 180) - (direct == 0))  * player_speed;
    /*   玩家坦克碰撞   */
    if (player_num - 1) {
        int posn_x = (*(player.rect.begin() + 2 - player_order)).x;
        int posn_y = (*(player.rect.begin() + 2 - player_order)).y;
       if (sqrt(pow(posn_x - pos_x, 2) + pow(posn_y - pos_y, 2)) < 2.0 * block_len) {
            return false;
        }
    }
    
    /*      敌人坦克碰撞         */
    for (std::vector<Tank::Tank_enemy>::iterator tmp = enemy.begin(); tmp != enemy.end(); tmp++) {
        int posn_x = (*tmp).rect.x;
        int posn_y = (*tmp).rect.y;
        if (sqrt(pow(posn_x - pos_x, 2) + pow(posn_y - pos_y, 2)) < 2.0 * block_len) {
            return false;
        }
    }


    pos_x = pos_x / block_len + (direct == 90 && pos_x % block_len) - (pos_x < 0);
    pos_y = pos_y / block_len + (direct == 180 && pos_y % block_len) - (pos_y < 0);

    /*   边界 / Heart  */
    if (pos_x < 0 || pos_x + 2 > block_num || pos_y < 0 || pos_y + 2 > block_num
        || (pos_y == block_num - 3 && (pos_x == block_num / 2 - 2 || pos_x == block_num / 2 - 1 || pos_x == block_num / 2))
        || (pos_y == block_num - 2 && (pos_x == block_num / 2 - 2 || pos_x == block_num / 2 - 1 || pos_x == block_num / 2)))
        return false;

    /*   碰撞   */
    if ((map_set[pos_y][pos_x].alive && map_set[pos_y][pos_x].element_kind != 3) ||
        (map_set[pos_y + 1][pos_x].alive && map_set[pos_y + 1][pos_x].element_kind != 3) ||
        (map_set[pos_y][pos_x + 1].alive && map_set[pos_y][pos_x + 1].element_kind != 3) ||
        (map_set[pos_y + 1][pos_x + 1].alive && map_set[pos_y + 1][pos_x + 1].element_kind != 3))
        return false;


    return true;
}


void CompletetPos(Tank::Tank_player &player, int player_order) {
    int x_now = (*(player.rect.begin() + player_order - 1)).x;
    int y_now = (*(player.rect.begin() + player_order - 1)).y;
    x_now = (x_now / block_len + (x_now % block_len > block_len / 2)) * block_len;
    y_now = (y_now / block_len + (y_now % block_len > block_len / 2)) * block_len;
    if (player_num - 1) {
        int posn_x = (*(player.rect.begin() + 2 - player_order)).x;
        int posn_y = (*(player.rect.begin() + 2 - player_order)).y;
        if (sqrt(pow(posn_x - x_now, 2) + pow(posn_y - y_now, 2)) < 2.0 * block_len) {
            return;
        }
    }
    (*(player.rect.begin() + player_order - 1)).x = (x_now / block_len + (x_now % block_len > block_len / 2)) * block_len;
    (*(player.rect.begin() + player_order - 1)).y = (y_now / block_len + (y_now % block_len > block_len / 2)) * block_len;
}


void Move_Tank_Player(Tank::Tank_player &player, int player_order, int move_direct, Map map_set[block_num][block_num], std::vector<Tank::Tank_enemy> &enemy) {
    if (!player_order) return;
    player.UpDateTankRect(player_order);
    /*             转向 或 移动             */
    move_direct = move_direct - 4 * (player_order - 1) - 1;    // 标准化 0-3
    if (*(player.direction.begin() + player_order - 1) != 90 * move_direct) {
        CompletetPos(player, player_order);                    // 补全位置     
        *(player.direction.begin() + player_order - 1) = 90 * move_direct;
    }
    else {
        if (!Can_Move_Player(player, player_order, map_set, move_direct, enemy)) return;
        (*(player.rect.begin() + player_order - 1)).x += ((move_direct == 1) - (move_direct == 3)) * player_speed;
        (*(player.rect.begin() + player_order - 1)).y += ((move_direct == 2) - (move_direct == 0)) * player_speed;
    }
}


void Draw_Tank(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    TankSurfaceTex &tank_sur_tex, SDL_Renderer *rend, int player_num, int key_state[],
    Map map_set[block_num][block_num]) {

    /*          玩家坦克移动             */
    for (int inc = _P1_MOVE_; inc <= std::min(_P2_MOVE_, player_num - 1); inc++) {
        if (player.player_reborn[inc].Is_overGap()) {
            Move_Tank_Player(player, (inc + 1) * (key_state[inc] > 0), key_state[inc], map_set, enemy);
        }
    }

    /*   绘制图象     */
    for (std::vector<Tank::Tank_enemy>::iterator inc = enemy.begin(); inc != enemy.end(); inc ++) {
        SDL_RenderCopyEx(rend, *(tank_sur_tex.tank_enemy_com_tex.begin() + (*inc).enemy_kind - 1),
            &(*inc).update_rect, &(*inc).rect, (*inc).direction, &(*inc).point, SDL_FLIP_NONE);
    }

    for (int idx = 0; idx < player_num; idx ++) {
        if (player.player_reborn[idx].Is_overGap()) {
            SDL_RenderCopyEx(rend, *(tank_sur_tex.tank_player_tex.begin() + idx),
                &*(player.update_rect.begin() + idx), &*(player.rect.begin() + idx), *(player.direction.begin() + idx),
                &*(player.point.begin() + idx), SDL_FLIP_NONE);
        }
    }
}


void Set_Player_Missile(std::vector<Tank::Tank_player::Missile> &player_missile, int key_state[], Tank::Tank_player &player) {
    Tank::Tank_player::Missile *tmp_Missile = new Tank::Tank_player::Missile;
    int list[] = { _P1_ATTACK_ , _P2_ATTACK_ };
    for (int inc = 0; inc < player.player_num; inc++) {
        if (key_state[list[inc]] && player.player_reborn[inc].Is_overGap()) {
            tmp_Missile->Set_missile(player, inc + 1);
            player_missile.push_back(*tmp_Missile);
            key_state[list[inc]] = false;
        }
    }

    delete tmp_Missile;
}


template <typename Missile>
void Map_Missile_hit(Missile &P_E_missile, Map map_set[block_num][block_num]) {

    typename Missile::iterator tmp = P_E_missile.begin();
    while (tmp != P_E_missile.end()) {
        (*tmp).rect.x += (((*tmp).direction == _EAST_) - ((*tmp).direction == _WEST_)) * missile_speed;
        (*tmp).rect.y += (((*tmp).direction == _SOUTH_) - ((*tmp).direction == _NORTH_)) * missile_speed;


        /*  边界 */
        if ((*tmp).rect.x < 0 || (*tmp).rect.x >= block_len * block_num - block_len / 2
            || (*tmp).rect.y < 0 || (*tmp).rect.y >= block_len * block_num - block_len / 2) {
            tmp = P_E_missile.erase(tmp);
            continue;
        }
        int tmp_x = (*tmp).rect.x / block_len;
        int tmp_y = (*tmp).rect.y / block_len;

        int such_x[] = { tmp_x, tmp_x + ((*tmp).direction == _NORTH_ || (*tmp).direction == _SOUTH_) };
        int such_y[] = { tmp_y, tmp_y + ((*tmp).direction == _EAST_ || (*tmp).direction == _WEST_) };
        /*  元素  */
        if (!map_set[tmp_y][tmp_x].alive && !map_set[such_y[1]][such_x[1]].alive) {
            tmp ++;
            continue;
        }

        bool be_erase = false;
        /*  毗邻元素 */
        for (int inc = 0; inc < 2; inc ++) {
            if (!map_set[such_y[inc]][such_x[inc]].alive) {
                continue;
            }
            switch (map_set[such_y[inc]][such_x[inc]].element_kind) {
            case _WALL_:
                map_set[such_y[inc]][such_x[inc]].alive = false;
                be_erase = true;
                break;
            case _STEEL_:
                be_erase = true;
                break;
            case _GRASS_:
                (*tmp).be_hide = true;
                break;
            default:
                (*tmp).be_hide = false;
                break;
            }
        }
        
        tmp = (be_erase ? P_E_missile.erase(tmp) : tmp + 1);   //消除炮弹
    }

}

void Kill_Missile_hit(std::vector<Tank::Tank_enemy::Missile> &enemy_missile, std::vector<Tank::Tank_player::Missile> &player_missile,
    std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player, std::vector<Timer> &enemy_reborn, blank &blanks) {

    /*  敌人 to 玩家  */
    std::vector<Tank::Tank_enemy::Missile>::iterator tmp = enemy_missile.begin();
    while (tmp != enemy_missile.end()) {
        bool be_erase = false;
        for (int player_order = 0; player_order < player.player_num; player_order ++) {
            if ((*tmp).rect.x >= player.rect[player_order].x && (*tmp).rect.x <= player.rect[player_order].x + 2 * block_len
                && (*tmp).rect.y >= player.rect[player_order].y && (*tmp).rect.y <= player.rect[player_order].y + 2 * block_len) {
                be_erase = true;
                Newborn_idx::player* players = new Newborn_idx::player;
                players -> Get_idx(player_order + 1, player.rect[player_order]);
                //player.invincible[player_order] = true;
                player.player_reborn[player_order].SetRecordClock();
                delete players;
                break;
            }
        }
        tmp = (be_erase ? enemy_missile.erase(tmp) : tmp + 1);
    }

    /*  玩家 to 敌人 */
    std::vector<Tank::Tank_player::Missile>::iterator temp = player_missile.begin();
    while (temp != player_missile.end()) {
        bool be_erase = false;
        std::vector<Tank::Tank_enemy>::iterator enem = enemy.begin();
        while (enem != enemy.end()) {
            if ((*temp).rect.x >= (*enem).rect.x && (*temp).rect.x <= (*enem).rect.x + 2 * block_len
                && (*temp).rect.y >= (*enem).rect.y && (*temp).rect.y <= (*enem).rect.y + 2 * block_len) {
                be_erase = true;
                enemy_reborn.push_back(Timer(enemy_reborn_gap, (*enem).born_idx));
                enem = enemy.erase(enem);
                blanks.killed_increase();
                break;
            }
            enem ++;
        }
        temp = (be_erase ? player_missile.erase(temp) : temp + 1);
    }
}


void HitDeter_player_or_enemy(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    std::vector<Timer> &enemy_reborn, Map map_set[block_num][block_num],
    std::vector<Tank::Tank_enemy::Missile> &enemy_missile, 
    std::vector<Tank::Tank_player::Missile> &player_missile, blank &blanks) {
    
    /*  移动炮弹 并 判断地图元素碰撞  */
    Map_Missile_hit(enemy_missile, map_set);
    Map_Missile_hit(player_missile, map_set);
    Kill_Missile_hit(enemy_missile, player_missile, enemy, player, enemy_reborn, blanks);
}


void FitEnemy(std::vector<Timer> &enemy_reborn, std::vector<Tank::Tank_enemy> &enemy, TankSurfaceTex &tank_sur_tex) {
    std::vector<int> dis_list;
    for (std::vector<Timer>::iterator tmp = enemy_reborn.begin(); tmp != enemy_reborn.end(); tmp ++) {
        if ((*tmp).Is_overGap()) {
            dis_list.push_back(tmp - enemy_reborn.begin());
            enemy.push_back(Tank::Tank_enemy((*tmp).GetMsg(), rand() % 3 + 1, tank_sur_tex));
        }
    }
    dis_list.reserve(dis_list.size());
    for (std::vector<int>::iterator tmp = dis_list.begin(); tmp != dis_list.end(); tmp++) {
        enemy_reborn.erase(*tmp + enemy_reborn.begin());
    }
}


void Draw_Missile(std::vector<Timer> &enemy_reborn, std::vector<Tank::Tank_enemy> &enemy,
    std::vector<Tank::Tank_player::Missile> &player_missile, TankSurfaceTex &tank_sur_tex,
    SDL_Renderer *rend, Tank::Tank_player &player, int key_state[], Map map_set[block_num][block_num],
    MapSurfaceTex &map_infor, std::vector<Tank::Tank_enemy::Missile>  &enemy_missile, blank &blanks) {

    std::vector<int> dis_list;
    /*       设置玩家新发射的炮弹          */
    Set_Player_Missile(player_missile, key_state, player);
    /*       绘制玩家和敌人发射炮弹              */
    /*  先进行炮弹动作处理 */
    HitDeter_player_or_enemy(enemy, player, enemy_reborn, map_set, enemy_missile, player_missile, blanks);

    SDL_Rect tmp_rect;
    tmp_rect.h = block_len;
    tmp_rect.w = block_len;

    /*  玩家炮弹  */
    for (std::vector<Tank::Tank_player::Missile>::iterator pl_tmp = player_missile.begin();
        pl_tmp != player_missile.end(); pl_tmp ++) {
        SDL_RenderCopy(rend, tank_sur_tex.player_missile_tex, NULL, &(*pl_tmp).rect);
        if ((*pl_tmp).be_hide) {    //覆盖隐藏
            (*pl_tmp).be_hide = false;
            tmp_rect.x = (*pl_tmp).rect.x / block_len * block_len;
            tmp_rect.y = (*pl_tmp).rect.y / block_len * block_len;
            if (map_set[tmp_rect.y / block_len][tmp_rect.x / block_len].element_kind == _GRASS_) {
                SDL_RenderCopy(rend, map_infor.grass_tex, NULL, &tmp_rect);
            }
            tmp_rect.x += ((*pl_tmp).direction == _NORTH_ || (*pl_tmp).direction == _SOUTH_) * block_len;
            tmp_rect.y += ((*pl_tmp).direction == _EAST_ || (*pl_tmp).direction == _WEST_) * block_len;
            if (map_set[tmp_rect.y / block_len][tmp_rect.x / block_len].element_kind == _GRASS_) {
                SDL_RenderCopy(rend, map_infor.grass_tex, NULL, &tmp_rect);
            }
        }
    }
    /*   敌人炮弹    */
    for (std::vector<Tank::Tank_enemy::Missile>::iterator en_tmp = enemy_missile.begin();
        en_tmp != enemy_missile.end(); en_tmp ++) {
        SDL_RenderCopy(rend, tank_sur_tex.enemy_missile_tex, NULL, &(*en_tmp).rect);
        if ((*en_tmp).be_hide) {    //覆盖隐藏
            (*en_tmp).be_hide = false;
            tmp_rect.x = (*en_tmp).rect.x / block_len * block_len;
            tmp_rect.y = (*en_tmp).rect.y / block_len * block_len;
            if (map_set[tmp_rect.y / block_len][tmp_rect.x / block_len].element_kind == _GRASS_) {
                SDL_RenderCopy(rend, map_infor.grass_tex, NULL, &tmp_rect);
            }
            tmp_rect.x += ((*en_tmp).direction == _NORTH_ || (*en_tmp).direction == _SOUTH_) * block_len;
            tmp_rect.y += ((*en_tmp).direction == _EAST_ || (*en_tmp).direction == _WEST_) * block_len;
            if (map_set[tmp_rect.y / block_len][tmp_rect.x / block_len].element_kind == _GRASS_) {
                SDL_RenderCopy(rend, map_infor.grass_tex, NULL, &tmp_rect);
            }
        }
    }

    /*  将enemy补全  */
    if (enemy_reborn.size() && (*(enemy_reborn.begin())).Is_overGap()) {
        FitEnemy(enemy_reborn, enemy, tank_sur_tex);
    }
}


void GameWorking(GameMode *gamemode) {
    srand((unsigned int)time(NULL));

    Map               map_set[block_num][block_num];
    Window_render     Init_window;
    TankSurfaceTex    tank_sur_tex(player_num, Init_window.rend);
    MapSurfaceTex     map_infor(Init_window.rend);
    Tank::Tank_player player(player_num, tank_sur_tex);
    Key_to_Num        key_to_num;
    blank             blanks;
    std::vector<Tank::Tank_enemy>           enemy;
    std::vector<Tank::Tank_enemy::Missile>  enemy_missile;
    std::vector<Tank::Tank_player::Missile> player_missile;
    std::vector<Timer> enemy_reborn;

    /*       选地图       */
    DecodeFromDat(gamemode -> GetMap_Order(), map_set);

    for (int inc = 0; inc < 3; inc++) { //配置enemy
        enemy.push_back(Tank::Tank_enemy(inc + 1, rand() % 3 + 1, tank_sur_tex));
    }
    /*            事件驱动循环              */
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            /*      更新操作信息      */
            quit = !key_to_num.SetKeyState(event, player);

            /*       特殊操作          */

        }
        /*      绘制背景 记分板      */
        SDL_RenderCopy(Init_window.rend, map_infor.background_tex, NULL, &map_infor.bcg_rect);
        SDL_RenderCopy(Init_window.rend, map_infor.scoreboard_tex, NULL, &map_infor.scb_rect);

        Cal_Tank_Enemy(enemy, player, tank_sur_tex, player_num, map_set, enemy_missile, player_missile);
        /*      绘制坦克      */
        Draw_Tank(enemy, player, tank_sur_tex, Init_window.rend, player_num, key_to_num.key_state, map_set);

        /*      绘制地图      */
        Draw_Map(map_set, map_infor, Init_window.rend);

        /*      绘制炮弹      */
        Draw_Missile(enemy_reborn, enemy, player_missile, tank_sur_tex, Init_window.rend, player, key_to_num.key_state, map_set, map_infor, enemy_missile, blanks);

        /*     更新渲染器  延时delay_time    */
        SDL_RenderPresent(Init_window.rend);
        SDL_Delay(delay_time);
    }

    SDL_Quit();
}
