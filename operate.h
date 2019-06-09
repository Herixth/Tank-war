/**
 * @File  operate.h
 * @Brief ����initialize.h
 *        ������Ϸ��̬������������
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
 * ���Ƶ�ͼ, ���¼�ѭ���е���
 * @param map_set   ������ͼԪ����Ϣ�Ķ�ά����, ͨ��������
 * @param map_infor ��������ͼԪ�صĶ���
 * @param rend      �����ڵ���Ⱦ��
 */
void Draw_Map(Map map_set[block_num][block_num], MapSurfaceTex& map_infor, SDL_Renderer* rend);


/**
 * �ж����̹���Ƿ�����ƶ�
 * ����: 
 *    1. �Ƿ��ƶ����߽�
 *    2. �Ƿ����ͼԪ����ײ
 *    3. �Ƿ�������̹����ײ
 *      ps: �������ֻ�ж��������̹���Ƿ���ײ
 *          ���̹�����ڵ�����ײ�ڱ�ĺ�������
 *    map_set[row][col]  ���� map_set[x][y]
 * @return true ���ƶ�; false �����ƶ�
 */ 
bool Can_Move_Player(Tank::Tank_player &player, int player_order, Map map_set[block_num][block_num], int move_direct, std::vector<Tank::Tank_enemy> &enemy);


/**
 * ��ȫλ��
 */
void CompletetPos(Tank::Tank_player &player, int player_order);


/**
 * �ƶ����̹��
 * @param player       �������̹����Ϣ
 * @param player_order ������ 1 / 2
 * @param move_direct  �²����ķ��� N,D,S,A -> 1,2,3,4 / UP.RIGHT,DOWN,LEFT -> 5,6,7,8
 * @param map_set      ��ͼԪ�ز���
 * @param enemy        ��������
 */
void Move_Tank_Player(Tank::Tank_player &player, int player_order, int move_direct, Map map_set[block_num][block_num], std::vector<Tank::Tank_enemy> &enemy);


/**
 * ����̹��, Ĭ��ͬʱ���ֵĵ�����Ŀ������3��, ���¼�ѭ���е���
 * ���ں����е����������������Ը�Ԫ�ؽ��д���
 * @param enemy        ����̹�˵�����vector, ����λ��,����,����,��������Ϣ��
 * @param player       ���̹�˵����ݶ���, �����Ա����λ��,����,��������Ϣ��
 * @param tank_sur_tex ��̹�˵�surface����
 * @param rend         ��������Ⱦ��
 * @param player_num   ������� 1/2
 * @param key_state    ����ʱ������
 * @param map_set      ��ͼԪ����Ϣ
 */
void Draw_Tank(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    TankSurfaceTex &tank_sur_tex, SDL_Renderer *rend, int player_num, int key_state[],
    Map map_set[block_num][block_num]);


/**
 * @brief ������Ҳ�������player_missile
 */
void Set_Player_Missile(std::vector<Tank::Tank_player::Missile> &player_missile, int key_state[], Tank::Tank_player &player);


/**
 * @brief �ڵ����жԵ�ͼԪ�ص���ײ������, ��Ҫ��rect���иı�
 */
template <typename Missile>
void Map_Missile_hit(Missile &P_E_missile, Map map_set[block_num][block_num]);


/**
 * @brief �ڵ�����ҶԵ��˵Ļ�ɱ���� 
 */
void Kill_Missile_hit(std::vector<Tank::Tank_enemy::Missile> &enemy_missile, std::vector<Tank::Tank_player::Missile> &player_missile,
    std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player, std::vector<Timer> &enemy_reborn, blank &blanks);


/**
 * @brief �������ڵ����д���
 *        ���ڵ���������,�Ի�����������ж�������  ����ڵ���grass������be_hideΪtrue
 * @param enemy  ����enemy��Ϣ���� λ�� ���� ���� ��
 * @param player ����player��Ϣ���� λ�� ���� �Ƿ��޵� �Ƿ������� ��
 * @param enemy_reborn �������õ��˵�����
 * @param map_set ��ͼԪ����Ϣ
 * @param enemy_missile  �����ڵ���Ϣ
 * @param player_missile ����ڵ���Ϣ
 */
void HitDeter_player_or_enemy(std::vector<Tank::Tank_enemy> &enemy, Tank::Tank_player &player,
    std::vector<Timer> &enemy_reborn, Map map_set[block_num][block_num],
    std::vector<Tank::Tank_enemy::Missile> &enemy_missile, 
    std::vector<Tank::Tank_player::Missile> &player_missile, blank &blanks);


/**
 * @brief ��ʱ�������������²������
 */
void FitEnemy(std::vector<Timer> &enemy_reborn, std::vector<Tank::Tank_enemy> &enemy, TankSurfaceTex &tank_sur_tex);


/**
 * @brief �����ڵ�
 */
void Draw_Missile(std::vector<Timer> &enemy_reborn, std::vector<Tank::Tank_enemy> &enemy,
    std::vector<Tank::Tank_player::Missile> &player_missile, TankSurfaceTex &tank_sur_tex,
    SDL_Renderer *rend, Tank::Tank_player &player, int key_state[], Map map_set[block_num][block_num],
    MapSurfaceTex &map_infor, std::vector<Tank::Tank_enemy::Missile>  &enemy_missile, blank &blanks);


void GameWorking(GameMode *gamemode);

#endif // !_OPERATE_


