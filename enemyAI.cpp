/**
 * @File  enemyAI.cpp
 * @Brief 用来进行enemyAI.h中各个函数的实现
 * @Date  2018/06
 */


#include "enemyAI.h"

/**
 * 判断位置属性
 */
int P_C_deter(Tank::Tank_enemy &single_enemy, Tank::Tank_player &player,
    int player_num, Map map_set[block_num][block_num],
    std::vector<Tank::Tank_player::Missile> &player_missile) {

    /*               MAD_POS                 */
    if (single_enemy.rect.y / block_len >= 22) {
        return MAD_POS;
    }

    /*               SPE_POS                   */
    for (int inc = 0; inc < player_num; inc++) {
        int x_tmp = (*(player.rect.begin() + inc)).x;
        int y_tmp = (*(player.rect.begin() + inc)).y;

        bool fail = false;
        bool cmp = false; //0 small 1 big
        if (abs(x_tmp - single_enemy.rect.x) < 2 * block_len) {            //在一列的判定范围内
            for (int row = std::min(y_tmp, single_enemy.rect.y); row < std::max(y_tmp, single_enemy.rect.y); row++) {
                if (map_set[row / block_len][single_enemy.rect.x / block_len].alive 
                    && (map_set[row / block_len][single_enemy.rect.x / block_len].element_kind == _WALL_
                    || map_set[row / block_len][single_enemy.rect.x / block_len].element_kind == _STEEL_)) {
                    fail = true;
                    cmp = (y_tmp > single_enemy.rect.y);
                    break;
                }
            }
            if (!fail) {
                return cmp ? SPE_POS_Y_SOUTH : SPE_POS_Y_NORTH;
            }
            else {
                fail = false;
            }
        }
        if (abs(y_tmp - single_enemy.rect.y) < 2 * block_len) {             //在一行的判定范围内
            for (int col = std::min(x_tmp, single_enemy.rect.x); col < std::max(x_tmp, single_enemy.rect.x); col++) {
                if (map_set[single_enemy.rect.y / block_len][col / block_len].alive 
                    && (map_set[single_enemy.rect.y / block_len][col / block_len].element_kind == _WALL_
                    || map_set[single_enemy.rect.y / block_len][col / block_len].element_kind == _STEEL_)) {
                    fail = true;
                    cmp = (x_tmp > single_enemy.rect.x);
                    break;
                }
            }
            if (!fail) {
                return cmp ? SPE_POS_X_EAST : SPE_POS_X_WEST;
            }
            else {
                fail = false;
            }
        }
    }
    /*  炮弹判定  */
    for (std::vector<Tank::Tank_player::Missile>::iterator tmp = player_missile.begin(); tmp != player_missile.end(); tmp++) {
        if ((*tmp).rect.x - single_enemy.rect.x >= 0 && (*tmp).rect.x - single_enemy.rect.x < 2 * block_len) {
            return (*tmp).rect.y > single_enemy.rect.y ? SPE_POS_Y_SOUTH : SPE_POS_Y_SOUTH;
        }
        if ((*tmp).rect.y - single_enemy.rect.y >= 0 && (*tmp).rect.y - single_enemy.rect.y < 2 * block_len) {
            return (*tmp).rect.x > single_enemy.rect.x ? SPE_POS_X_EAST : SPE_POS_X_WEST;
        }
    }

    int tmp_x = (single_enemy.rect.x + ((single_enemy.direction == _EAST_) - (single_enemy.direction == _WEST_)) * enemy_speed) / block_len;
    int tmp_y = (single_enemy.rect.y + ((single_enemy.direction == _SOUTH_) - (single_enemy.direction == _NORTH_)) * enemy_speed) / block_len;

    if ((map_set[tmp_y][tmp_x].alive && map_set[tmp_y][tmp_x].element_kind != _GRASS_)
        || (map_set[tmp_y + 1][tmp_x].alive && map_set[tmp_y + 1][tmp_x].element_kind != _GRASS_)
        || (map_set[tmp_y][tmp_x + 1].alive && map_set[tmp_y][tmp_x + 1].element_kind != _GRASS_)
        || (map_set[tmp_y + 1][tmp_x + 1].alive && map_set[tmp_y + 1][tmp_x + 1].element_kind != _GRASS_)) {
        int direct[] = { _NORTH_, _EAST_, _SOUTH_, _WEST_ };
        double posi[4] = { v_L, tmp_x < block_num / 2 ? v_M : v_L, v_M, tmp_x >= block_num / 2 ? v_M : v_L };
        
        for (int inc = 0; inc < 4; inc++) {
            if (direct[inc] == single_enemy.direction) {
                posi[inc] = 0.0;
            }
        }

        int rand_num = rand() % area;
        double tmp_s = 0.0;
        for (int inc = 0; inc < 4; inc++) {
            if (direct[inc] == single_enemy.direction) {
                continue;
            }
            tmp_s = tmp_s + (posi[inc] / (2 * v_L + v_M)) * area;
            if (rand_num < tmp_s) {
                int ans[] = { SPE_POS_Y_NORTH, SPE_POS_X_EAST, SPE_POS_Y_SOUTH, SPE_POS_X_WEST };
                return ans[inc];
            }
        }
    }

    /*               COM_POS               */
    return COM_POS;
}

/**
 * 判断目的
 */
int A_P_deter(int P_C) {
    double rand_num = rand() % area;
    double part_posi_list[] = { 0.0, 0.0, 0.0, 0.0 };
    double frac_pr_list = 0.0;       //分布函数
    double denomi = 0.0;
    
    /*        <A>   <B>   <C>   <D>      */
    switch (P_C) {
    case COM_POS:
        denomi = v_H + 0 + v_M * Lo_v_M + v_L * Lo_v_L;
        part_posi_list[0] = v_H / denomi;
        part_posi_list[1] = 0   / denomi;
        part_posi_list[2] = v_M * Lo_v_M / denomi;
        part_posi_list[3] = v_L * Lo_v_L / denomi;
        for (int inc = 0; inc < 4; inc ++) {
            frac_pr_list = frac_pr_list + area * part_posi_list[inc];
            if (rand_num <= frac_pr_list) {
                rand_num = rand() % area;
                switch (inc) {
                case 0:    //A
                    return rand_num < area * Lo_v_H ? _A_1 : _A_2;
                    break;
                case 2:    //C
                    return _C_2;
                    break;
                default:   //D
                    return _D_1;
                    break;
                }
            }
        }
        break;
    case MAD_POS:
        denomi = v_M + 0 + v_H + v_L;
        part_posi_list[0] = v_M / denomi;
        part_posi_list[1] = 0   / denomi;
        part_posi_list[2] = v_H / denomi;
        part_posi_list[3] = v_L / denomi;
        for (int inc = 0; inc < 4; inc++) {
            frac_pr_list = frac_pr_list + area * part_posi_list[inc];
            if (rand_num <= frac_pr_list) {
                rand_num = rand() % area;
                switch (inc) {
                case 0:    //A
                    return rand_num < area * Lo_v_H ? _A_1 : _A_2;
                    break;
                case 2:    //C
                    return rand_num < area * Lo_v_M ? _C_1 : _C_2;
                    break;
                default:   //D
                    return rand_num < area * Lo_v_M ? _D_1 : _D_2;
                    break;
                }
            }
        }
        break;
    case SPE_POS_X_EAST: case SPE_POS_X_WEST: case SPE_POS_Y_NORTH: case SPE_POS_Y_SOUTH:
        denomi = v_H + v_H + v_H + v_M;
        part_posi_list[0] = v_H / denomi;
        part_posi_list[1] = v_H / denomi;
        part_posi_list[2] = v_M / denomi;
        part_posi_list[3] = v_M / denomi;
        for (int inc = 0; inc < 4; inc++) {
            frac_pr_list = frac_pr_list + area * part_posi_list[inc];
            if (rand_num <= frac_pr_list) {
                rand_num = rand() % area;
                switch (inc) {
                case 0:    //A
                    return rand_num < area * Lo_v_H ? _A_1 : _A_2;
                    break;
                case 1:   //B
                    return _B_1;
                    break;
                case 2:    //C
                    return rand_num < area * Lo_v_H ? _C_1 : _C_2;
                    break;
                default:   //D
                    return rand_num < area * Lo_v_M ? _D_1 : _D_2;
                    break;
                }
            }
        }
        break;
    default:
        break;
    }
    return 0;
}

/**
 * 判断基础行为
 */
int B_A_deter(int A_P, int P_C) {
    int turn_list[] = { _TAK_TURN_NORTH, _TAK_TURN_EAST, _TAK_TURN_SOUTH, _TAK_TURN_WEST };
    int fire_list[] = { _TAK_FIRE_NORTH, _TAK_FIRE_EAST, _TAK_FIRE_SOUTH, _TAK_FIRE_WEST };

    if (P_C >= SPE_POS_Y_NORTH && P_C <= SPE_POS_X_WEST) {    // Spe_pos
        if (A_P == _A_1 || A_P == _A_2) {
            return turn_list[P_C - SPE_POS_Y_NORTH];
        }
        else {
            return fire_list[P_C - SPE_POS_Y_NORTH];
        }
    }

    if (P_C == COM_POS) {
        return _TAK_KEEP;
    }
    switch (A_P) {
    case _D_1:
        return _TAK_WAIT;
        break;
    case _A_1:
        
        break;
    case _A_2:
        
        break;
    case _B_1:
        
        break;
    case _C_1:
        
        break;
    case _C_2:
        
        break;
    case _D_2:
        
        break;
    default:
        break;
    }
    return 0;
}

void Reality_enemy(int B_A, Tank::Tank_enemy &single_enemy, std::vector<Tank::Tank_enemy::Missile> &missile, 
    Tank::Tank_player &player, Map map_set[block_num][block_num]) {
    int directions[] = { _NORTH_, _EAST_, _SOUTH_, _WEST_ };
    switch (B_A) {
    case _TAK_WAIT:
        return;
        break;
    case _TAK_TURN_NORTH: case _TAK_TURN_EAST: case _TAK_TURN_SOUTH: case _TAK_TURN_WEST: case _TAK_KEEP:
        if (B_A != _TAK_KEEP) {
            single_enemy.direction = directions[B_A - _TAK_TURN_NORTH];
            CompletetPos(single_enemy);
        }

        if (Can_move_enemy(player, map_set, single_enemy)) {
            Enemy_move(single_enemy);
        }
        else {
            /*        尽头转向     */
            std::vector<int> mid_direct;
            switch (single_enemy.direction) {
            case _NORTH_: case _SOUTH_:
                if (single_enemy.rect.x / block_len >= block_num / 2) {
                    mid_direct.push_back(_WEST_);
                    mid_direct.push_back(_EAST_);
                }
                else {
                    mid_direct.push_back(_EAST_);
                    mid_direct.push_back(_WEST_);
                }
                break;
            case _EAST_: case _WEST_:
                mid_direct.push_back(_SOUTH_);
                mid_direct.push_back(_NORTH_);
                break;
            }
            mid_direct.push_back((single_enemy.direction + _SOUTH_) % (2 * _SOUTH_));
            for (int count = 0; !Can_move_enemy(player, map_set, single_enemy) && count < mid_direct.size(); count ++) {
                single_enemy.direction = mid_direct[count];
            }
            CompletetPos(single_enemy);
            Enemy_move(single_enemy);
        }
        break;
    case _TAK_FIRE_NORTH: case _TAK_FIRE_EAST: case _TAK_FIRE_SOUTH: case _TAK_FIRE_WEST:
        if ((B_A - _TAK_FIRE_NORTH) * _EAST_ != single_enemy.direction) {
            single_enemy.direction = directions[B_A - _TAK_FIRE_NORTH];
            CompletetPos(single_enemy);
            Enemy_move(single_enemy);
        }
        if (single_enemy.enemy_missile_timer.Is_overGap()) {
            missile.push_back(Tank::Tank_enemy::Missile(single_enemy.direction, single_enemy.enemy_kind, single_enemy.rect.x, single_enemy.rect.y));
            single_enemy.enemy_missile_timer.SetRecordClock();
        }
        break;
    default:
        if (Can_move_enemy(player, map_set, single_enemy)) {
            Enemy_move(single_enemy);
        }
        break;
    }
}

bool Can_move_enemy(Tank::Tank_player &player, Map map_set[block_num][block_num], Tank::Tank_enemy &single_enemy) {
    int pos_x = single_enemy.rect.x;
    int pos_y = single_enemy.rect.y;
    int direct = single_enemy.direction;
    pos_x += ((direct == _EAST_) - (direct == _WEST_)) * enemy_speed;
    pos_y += ((direct == _SOUTH_) - (direct == _NORTH_))  * enemy_speed;
    /*   玩家坦克碰撞   */
    for (int inc = 0; inc < player.player_num; inc ++) {
        int posn_x = (*(player.rect.begin() + inc)).x;
        int posn_y = (*(player.rect.begin() + inc)).y;
        if (sqrt(pow(posn_x - pos_x, 2) + pow(posn_y - pos_y, 2)) < 2.0 * block_len) {
            return false;
        }
    }

    /*      敌人坦克碰撞         */

    pos_x = pos_x / block_len + (direct == _EAST_ && pos_x % block_len) - (pos_x < 0);
    pos_y = pos_y / block_len + (direct == _SOUTH_ && pos_y % block_len) - (pos_y < 0);

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

/**
 * 位置移动
 */
void Enemy_move(Tank::Tank_enemy &single_enemy) {
    single_enemy.rect.x += ((single_enemy.direction == _EAST_) - (single_enemy.direction == _WEST_)) * enemy_speed;
    single_enemy.rect.y += ((single_enemy.direction == _SOUTH_) - (single_enemy.direction == _NORTH_)) * enemy_speed;
}

/**
* 补全位置
*/
void CompletetPos(Tank::Tank_enemy &single_enemy) {
    int x_now = single_enemy.rect.x;
    int y_now = single_enemy.rect.y;
    x_now = (x_now / block_len + (x_now % block_len > block_len / 2)) * block_len;
    y_now = (y_now / block_len + (y_now % block_len > block_len / 2)) * block_len;
   
    /* 转向BUG 修复*/


    single_enemy.rect.x = (x_now / block_len + (x_now % block_len > block_len / 2)) * block_len;
    single_enemy.rect.y = (y_now / block_len + (y_now % block_len > block_len / 2)) * block_len;
}

/**
 * 汇总
 */
void Cal_Tank_Enemy(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    TankSurfaceTex &tank_sur_tex, int player_num, Map map_set[block_num][block_num],
    std::vector<Tank::Tank_enemy::Missile> &enemy_missile, std::vector<Tank::Tank_player::Missile> &player_missile) {

    //Timer update_timer(update_gap);

    for (std::vector<Tank::Tank_enemy>::iterator tmp = enemy.begin(); tmp != enemy.end(); tmp++) {
        int P_C = P_C_deter(*tmp, player, player_num, map_set, player_missile);
        int A_P = A_P_deter(P_C);
        int B_A = B_A_deter(A_P, P_C);
        Reality_enemy(B_A, *tmp, enemy_missile, player, map_set);

    }

}
