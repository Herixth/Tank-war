/**
 * @File  scoreboard.cpp
 * @Brief ��������scoreboard.h�и����༰���������ʵ��
 * @Date  2018/05
 */

#include "scoreboard.h"

blank::blank() {
    enemy_has_killed = 0;
}

blank::~blank() {

}

void blank::show_killed() {
    std::cout << enemy_has_killed << std::endl;
}

void blank::killed_increase() {
    enemy_has_killed ++;
}