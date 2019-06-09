/**
 * @File  operate.h
 * @Brief 包含initialize.h
 *        多种游戏动态操作函数和类
 * @Date  2018/05
 */


#pragma once
#ifndef _OPERATE_
#define _OPERATE_

#include "initialize.h"
#include "scoreboard.h"
#include "decoder.h"
#include "enemyAI.h"

/**
 * 绘制地图, 在事件循环中调用
 * @param map_set   包含地图元素信息的二维数组, 通过解码获得
 * @param map_infor 包含各地图元素的对象
 * @param rend      主窗口的渲染器
 */
void Draw_Map(Map map_set[block_num][block_num], MapSurfaceTex& map_infor, SDL_Renderer* rend);


/**
 * 判断玩家坦克是否可以移动
 * 考虑: 
 *    1. 是否移动到边界
 *    2. 是否与地图元素碰撞
 *    3. 是否与其他坦克碰撞
 *      ps: 这个函数只判断两个玩家坦克是否碰撞
 *          玩家坦克与炮弹的碰撞在别的函数考虑
 *    map_set[row][col]  不是 map_set[x][y]
 * @return true 可移动; false 不能移动
 */ 
bool Can_Move_Player(Tank::Tank_player &player, int player_order, Map map_set[block_num][block_num], int move_direct, std::vector<Tank::Tank_enemy> &enemy);


/**
 * 补全位置
 */
void CompletetPos(Tank::Tank_player &player, int player_order);


/**
 * 移动玩家坦克
 * @param player       包含玩家坦克信息
 * @param player_order 玩家序号 1 / 2
 * @param move_direct  新操作的方向 N,D,S,A -> 1,2,3,4 / UP.RIGHT,DOWN,LEFT -> 5,6,7,8
 * @param map_set      地图元素布局
 * @param enemy        敌人序列
 */
void Move_Tank_Player(Tank::Tank_player &player, int player_order, int move_direct, Map map_set[block_num][block_num], std::vector<Tank::Tank_enemy> &enemy);


/**
 * 绘制坦克, 默认同时出现的敌人数目不超过3个, 在事件循环中调用
 * 并在函数中调用其它操作函数对各元素进行处理
 * @param enemy        敌人坦克的数据vector, 包含位置,方向,种类,发射物信息等
 * @param player       玩家坦克的数据对象, 对象成员包含位置,方向,发射物信息等
 * @param tank_sur_tex 各坦克的surface对象
 * @param rend         主窗口渲染器
 * @param player_num   玩家数量 1/2
 * @param key_state    操作时间数组
 * @param map_set      地图元素信息
 */
void Draw_Tank(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    TankSurfaceTex &tank_sur_tex, SDL_Renderer *rend, int player_num, int key_state[],
    Map map_set[block_num][block_num]);


/**
 * @brief 根据玩家操作设置player_missile
 */
void Set_Player_Missile(std::vector<Tank::Tank_player::Missile> &player_missile, int key_state[], Tank::Tank_player &player);


/**
 * @brief 炮弹序列对地图元素的碰撞及处理, 需要对rect进行改变
 */
template <typename Missile>
void Map_Missile_hit(Missile &P_E_missile, Map map_set[block_num][block_num]);


/**
 * @brief 炮弹对玩家对敌人的击杀处理 
 */
void Kill_Missile_hit(std::vector<Tank::Tank_enemy::Missile> &enemy_missile, std::vector<Tank::Tank_player::Missile> &player_missile,
    std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player, std::vector<Timer> &enemy_reborn, blank &blanks);


/**
 * @brief 对所有炮弹进行处理
 *        对炮弹进行消除,对击中情况进行判定并处理  如果炮弹在grass中设置be_hide为true
 * @param enemy  包含enemy信息序列 位置 方向 种类 等
 * @param player 包含player信息序列 位置 方向 是否无敌 是否在重生 等
 * @param enemy_reborn 额外设置敌人的重生
 * @param map_set 地图元素信息
 * @param enemy_missile  敌人炮弹信息
 * @param player_missile 玩家炮弹信息
 */
void HitDeter_player_or_enemy(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    std::vector<Timer> &enemy_reborn, Map map_set[block_num][block_num],
    std::vector<Tank::Tank_enemy::Missile> &enemy_missile, 
    std::vector<Tank::Tank_player::Missile> &player_missile, blank &blanks);


/**
 * @brief 在时间间隔满足条件下补充敌人
 */
void FitEnemy(std::vector<Timer> &enemy_reborn, std::vector<Tank::Tank_enemy> &enemy, TankSurfaceTex &tank_sur_tex);


/**
 * @brief 绘制炮弹
 */
void Draw_Missile(std::vector<Timer> &enemy_reborn, std::vector<Tank::Tank_enemy> &enemy,
    std::vector<Tank::Tank_player::Missile> &player_missile, TankSurfaceTex &tank_sur_tex,
    SDL_Renderer *rend, Tank::Tank_player &player, int key_state[], Map map_set[block_num][block_num],
    MapSurfaceTex &map_infor, std::vector<Tank::Tank_enemy::Missile>  &enemy_missile, blank &blanks);


void GameWorking(GameMode *gamemode);

#endif // !_OPERATE_


