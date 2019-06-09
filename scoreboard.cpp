/**
 * @File  scoreboard.cpp
 * @Brief 用来进行scoreboard.h中各个类及函数定义的实现
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