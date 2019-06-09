/**
 * @File  scoreboard.h
 * @Brief 包含游戏过程中的记分类及函数
 * 该文件的类或函数在其他文件中嵌入式使用
 * @Date 2018/05
 */


#pragma once
#ifndef _SCOREBOARD_
#define _SCOREBOARD_

#include "menu.h"

class blank {
public:
    blank();
    ~blank();
    void show_killed();
    void killed_increase();
private:
    int enemy_has_killed;
};


#endif // !_SCOREBOARD_
