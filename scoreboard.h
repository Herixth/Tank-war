/**
 * @File  scoreboard.h
 * @Brief ������Ϸ�����еļǷ��༰����
 * ���ļ���������������ļ���Ƕ��ʽʹ��
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
