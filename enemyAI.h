/**
 * @File  enemyAI.h
 * @Brief ��������enemy��ĳ��ʱ�̵���Ϊ
 * ���ø����㷨����AIʵ��
 * ̹�˻��ƺ�����operate.h�ж��塣������о���Ĳ���
 * @Date  2018/05
 */

#pragma once
/**
 * @File enemyAI.h
 *  ���������������enemy AI��Ϊ�ĺ���
 * 
 *   Script:   
 *       
 *       Symbol defination:
 *           - .H -> High level  \ .M -> Medium \ .L -> Low 
 *           - .Inf -> known information \ .Ope operation
 *           - Cal(param) -> completion of calculation of param : @param expression
 *           - Ex(param)  -> expectation of param : @param expression
 *           - V(param)   -> priority level : @param level(.H, .M, .L) 
 *           - Lo(param)  -> local scope    : @param param
 *           - Fun(param) -> undefined function : @param param
 *--------------------------------------------------------------------------------------
 *       Advanced_Purpose owned by enemy:             @A_P
 *         A> PathMoving_state        Expectation
 *             -1 Move_path_heart      Lo(V(.H))
 *             -2 Move_path_player     Lo(V(.L))
 *         
 *         B> Attack_state
 *             -1 Attack_player        
 *             
 *         C> Failure_state
 *             -1 Wall_break           Lo(V(.M))
 *             -2 Heart_shoot          Lo(V(.M))
 *         
 *         D> Defense_state
 *             -1 Move_keep            Lo(V(.M))
 *             -2 Mis_offset           Lo(V(.M))
 *--------------------------------------------------------------------------------------------------------------   
 *       Basic_Action of different A_P:               @B_A
 *         - Tak_keep    ��    { <A>PathMoving_state }
 *         - Tak_turn    ��    { <A>PathMoving_state }
 *         - Tak_wait    ��    { <D>[1]Defense_state - Move_keep }
 *         - Mis_fire    ��    { <B>Attack_player , <C>Failure_state , <D>[2]Defense_state - Mis_offset }  
 *--------------------------------------------------------------------------------------------------------------   
 *       Position_Classification determined by enemy: @P_C
 *             Name           Ex(P_C[])       Ex(Cal[])         A_P sort
 *         -a Com_pos           .H              .M            A > C > B > D
 *         -b Spe_pos           .M              .H            B > D > A > C
 *         -c Mad_pos           .L              .L            C > A > B > D
 *--------------------------------------------------------------------------------------------------------------   
 *       Flow Chart:
 *               |------ < ------ < ------ < ------ < ------ < ------ < ------ < ------ < ------ < ------ < --
 *           Status_Now                                                                                      ��
 *               |                                                                                           |
 *               |     Step                       .Inf                                  .Ope                 |
 *               V      a.         enemy, player, player_missile, map_set         Fun_a(.Inf)  => P_C        ��
 *               |      b.                     .Inf(a), P_C                       Fun_b(.Inf)  => A_P        |
 *               |      c.                     .Inf(a), A_P                       Fun_c(.Inf)  => B_A        |
 *               V      d.                     .Inf(a), B_A                       Fun_d(B_A)   =>        New_Status
 *               |                                                                                           �� 
 *               ------ > ------ > ------ > ------ > ------ > ------ > ------ > ------ > ------ > ------ > --|   
 *--------------------------------------------------------------------------------------------------------------       
 *       Functions:
 *           1. Fun_a(.Inf) => P_C
 *                  Brief: Input an enemy vector iterator and other information 
 *                      According to it's absolute position and relative position to output P_C(Com_pos, Spe_pos, Mad_pos)
 *                  
 *                  Param:
 *                      class Tank::Tank_enemy  include { rect, direction, enemy_kind } needed.  
 *                      class Tank::Tank_player include { vector<rect>, vector<direction>, player_num } needed.
 *                      vector<Tank::Tank_player::Missile> player_missile include list of { rect, direction } needed.
 *                      Map **map_set include list of { alive, element_kind } needed.
 *              
 *                  Details:
 *                      -a Com_pos    except Spe_pos and Mad_pos
 *
 *                      -b Spe_pos    1.player and enemy are in the same line
 *                                    2.player_missile around
 *
 *                      -c Mad_pos    1.close to Heart    
 *                    
 *                   Return:
 *                      A P_C from { Com_pos, Spe_pos, Mad_pos }
 *
 *           2. Fun_b(.Inf) => A_P
 *                  Brief: Input P_C with some other information to choose A_P and return it
 *
 *                  Param: 
 *                      P_C Com_pos/Spe_pos/Mad_pos
 *                      class Tank::Tank_enemy  include { rect, direction, enemy_kind } needed.  
 *                      class Tank::Tank_player include { vector<rect>, vector<direction>, player_num } needed.
 *                      vector<Tank::Tank_player::Missile> player_missile include list of { rect, direction } needed.
 *                      Map **map_set include list of { alive, element_kind } needed.
 *
 *                  Details:
 *                      switch(P_C) 
 *                         -a Com_pos:
 *                               ans = V(.H) * (Lo(V(.H)) * <A>[1] + Lo(V(.L)) * <A>[2]) 
 *                                       + ((Wall in line == true) * Lo(V(.M)) * <C>[1] + Lo(V(.L)) * <C>[2]) 
 *                                       + (Player in line == true) * V(.L) * <B>[1]
 *                                       + V(.L) * (<D>[1] + (Player_missile in line == true) * <D>[2])
 *                         -b Spe_pos:
 *                               ans =  V(.H) * <B>[1] 
 *                                       + V(.M) * (Lo(V(.M)) * <D>[1] + (Player_missile in line == true) * Lo(V(.M)) * <D>[2])
 *                                       + V(.M) * (Lo(V(.H)) * <A>[1] + Lo(V(.L))) 
 *                                       + ((Wall in line == true) * Lo(V(.L)) * <C>[1] + Lo(V(.L) * <C>[2])
 *                         -c Mad_pos:
 *                               ans =  ((Wall in line == true) * Lo(V(.H)) * <C>[1] + (Heart in line == true) * Lo(V(.H)))
 *                                       + (Lo(V(.H)) * <A>[1] + Lo(V(.L)) * <A>[2])
 *                                       + (Player in line == true) * V(.M) * B[1]
 *                                       + V(.L) * (<D>[1] + (Player_missile in line == true) * <D>[2])
 * 
 *           3. Fun_c(.Inf) => B_A
 *                  Brief: a linear function which can describe the set from A_P to B_A
 * 
 *                  Param: 
 *                      A_P { <A>[1], <A>[2], <B>[1], <C>[1], <C>[2], <D>[1], <D>[2] }
 *                      class Tank::Tank_enemy  include { rect, direction, enemy_kind } needed.  
 *                      Map **map_set include list of { alive, element_kind } needed.
 *
 *                  Details:
 *                      <A>[1], <A>[2]: Direction can move: V(.H) * Tak_keep + V(.L) * Tak_turn
 *                                      else: Tak_turn
 *
 *                      <B>[1], <B>[2]: Mis_fire
 * 
 *                      <C>[1], <C>[2]: Mis_fire
 * 
 *                      <D>[1]: Tak_wait
 * 
 *                      <D>[2]: Mis_fire
 *                      
 *
 * @Date 2018/05
 */


#ifndef _ENEMY_AI_
#define _ENEMY_AI_

#include "initialize.h"

/*     P_C      */
#define COM_POS 0x1
#define SPE_POS_X_EAST  0x3
#define SPE_POS_X_WEST  0x5
#define SPE_POS_Y_NORTH 0x2
#define SPE_POS_Y_SOUTH 0x4
#define MAD_POS 0x6

/*     A_P      */
#define _A_1 0x1
#define _A_2 0x2
#define _B_1 0x3
#define _C_1 0x4
#define _C_2 0x5
#define _D_1 0x6
#define _D_2 0x7

/*     B_A     */
#define _TAK_KEEP       0X1
#define _TAK_TURN_NORTH 0X2
#define _TAK_TURN_EAST  0X3
#define _TAK_TURN_SOUTH 0X4
#define _TAK_TURN_WEST  0X5
#define _TAK_WAIT       0X6
#define _TAK_FIRE_NORTH 0X7
#define _TAK_FIRE_EAST  0X8
#define _TAK_FIRE_SOUTH 0X9
#define _TAK_FIRE_WEST  0Xa

const int area   = 10000;   //������ֵ
const double v_H = 0.7;
const double v_M = 0.3;
const double v_L = 0.1;
const double Lo_v_H = 0.8;
const double Lo_v_L = 0.1;
const double Lo_v_M = 0.5;

const double update_gap = 0.01;


/**
 * @brief ������enemy����Ϊ���л���
 * ��ҪĿ�������ֲ���:
 *                 1.enemy�������ƶ�
 *                 2.enemy�ڵ��ķ���
 * @param enemy  ����̹�˵�λ����Ϣ,������Ϣ��
 * @param player ��ҵĻ�����Ϣ
 * @param tank_sur_tex   ��������̹�˼��ڵ���surface,texture
 * @param palyer_num     ������� 1/2
 * @param map_set        ��ͼԪ�ز�����Ϣ
 * @param enemy_missile  ̹���ڵ�����
 * @param player_missile ����ڵ�����
 */
void Cal_Tank_Enemy(std::vector<Tank::Tank_enemy> &, Tank::Tank_player &, TankSurfaceTex &, int, Map[block_num][block_num],
    std::vector<Tank::Tank_enemy::Missile> &, std::vector<Tank::Tank_player::Missile> &);

/**
 * �ж�λ������
 * @return COM_POS, SPE_POS_X_EAST, SPE_POS_X_WEST, SPE_POS_y_NORTH, SPE_POS_y_SOUTH, MAD_POS
 */
int P_C_deter(Tank::Tank_enemy &single_enemy, Tank::Tank_player &player,
    int player_num, Map map_set[block_num][block_num],
    std::vector<Tank::Tank_player::Missile> &player_missile);

/**
 * �ж�Ŀ��
 * @return _A_1, _A_2, _B_1, _C_1, _C_2, _D_1, _D_2
 */
int A_P_deter(int P_C);

/**
 * �жϻ�����Ϊ
 * @return _TAK_KEEP, _TAK_TURN_NORTH, _TAK_TURN_EAST, _TAK_TURN_SOUTH, _TAK_TURN_WEST, _TAK_WAIT,
 *         _TAK_FIRE_NORTH, _TAK_FIRE_EAST, _TAK_FIRE_SOUTH, _TAK_FIRE_WEST
 */
int B_A_deter(int A_P, int P_C);

/**
 * ʵ�ֻ�����Ϊ
 * @param single_enemy �������˵ķ������ƶ�
 * @param missile �����ڵ�����
 */
void Reality_enemy(int, Tank::Tank_enemy &, std::vector<Tank::Tank_enemy::Missile> &, Tank::Tank_player &, Map [block_num][block_num]);

/**
 * �жϵ����Ƿ���ش˷����ƶ�
 */
bool Can_move_enemy(Tank::Tank_player &, Map [block_num][block_num], Tank::Tank_enemy &);

/**
 * ��ȫλ��
 */
void CompletetPos(Tank::Tank_enemy &);

/**
 * �򵥵�λ���ƶ�
 */
void Enemy_move(Tank::Tank_enemy &);

#endif // !_ENEMY_AI_

