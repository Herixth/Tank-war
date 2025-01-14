/**
 * @File  源.cpp
 * @Brief 包含主函数，集成类与函数进行操作
 * @Date  2018-05-10
 */


//#include "menu.h"
#include "operate.h"
#include "decoder.h"
#include "initialize.h"
#include "scoreboard.h"

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
        GameWorking(&MenuWorking());
    }
	return 0;
}

/**********************************项目日志***************************************/

/**
 * @日期: 2018-05-16 (Day_6)
 * 
 * @更新内容:
 *     1.增加了坦克转向时的位置修复函数,保证坦克不会出现在非法位置
 *       即不论何时,坦克的位置坐标最多只有一个边不是block_len的整数倍
 *
 * @待解决问题: 
 *     1.多事件处理   -- 在同一时刻只能进行一种操作                  /solved
 *     2.坦克移动动画 -- 坦克在屏幕上平移没有动态效果                 /solved
 *     3.记分板设计   -- 窗口右侧记分板空间没有处理
 *     4.菜单设计     -- 游戏开始没有选项菜单
 *     5.P1移动操作   -- P1坦克的移动只能在输入法打开条件下操作      /solved
 * 
 * @对应解决方案:
 *     1.查资料和SDL_PushEvent()函数及事件队列有关                   /to Day_7
 *     2.移动时对图像的粘贴进行更改,可以只对坦克履带的像素进行更改      /to Day_11
 *     3.暂不考虑
 *     4.暂不考虑
 *     5.暂不考虑                                                    /to Day_7
 */

/**
 * @日期: 2018-05-17 (Day_7)
 *
 * @更新内容:
 *     1.增加Key_to_Num类,用于优化数组记录过程
 *     2.利用数组记录优化了多事件处理问题:
 *           利用map建立SDLK到int的映射<见"initialize.cpp" row >= 200>
 *           根据映射关系建立key_state记录数组<由类函数SetKeyState实现,同时引入鼠标事件>
 *     3.根据新建立的事件记录关系改写"operate.h"中Draw_Tank函数实现多事件操作
 *     4.增加了二人玩家坦克的碰撞判定
 *
 * @待解决问题:
 *     1.对于单个玩家坦克,方向的移动不存在等待序列                       /solved
 *     -- 即在按住W后又按住D,坦克向W的方向走,但是在松开W而不松开D时,坦克不会转向D而停止
 *     2.判断玩家坦克间的碰撞存在bug                                     /solved
 *     -- 在非block_len整倍数点临界碰撞后转向,由于存在位置修复函数可能会导致坦克卡在一起
 *     3.动画效果太弱  -- 缺少坦克的生成动画
 *
 * @对应解决方案:
 *     1.增加一个针对P1,P2移动操作的等待值                               /to Day_7
 *     2.考虑增强位置修复函数的功能                                      /to Day_7
 *     3.暂不考虑
 */

/**
 * @日期: 2018-05-18 (Day_8)
 *
 * @更新内容:
 *     1.增加了玩家炮弹的发射模型
 *
 * @待解决问题:
 *     1.炮弹的发射间隙没有设置              /solved
 *     2.炮弹的碰撞没有判定完全        
 *
 * @对应解决方案:
 *     1.增加Timer计时器类进行操作           /to Day_9
 *     2.重新优化Draw_Missile函数
 */

/**
 * @日期: 2018-05-19 (Day_9)
 *
 * @更新内容:
 *     1.增加了玩家炮弹的地图碰撞模型
 *     2.优化了玩家炮弹发射间隔
 *
 * @待解决问题:
 *     1.炮弹在草丛中应该隐藏               /solved
 *     2.缺少炮弹击中目标的动画效果
 *
 * @对应解决方案:
 *     1.考虑更改Draw函数机制               /to Day_13
 *     2.暂不考虑
 */

/**
 * @日期: 2018-05-20 (Day_10)
 *
 * @更新内容:
 *     1.增加了玩家坦克移动动态效果
 *     2.定义了menu.h头文件用来描述菜单界面
 *
 * @待解决问题:
 *     1.玩家坦克移动动态效果帧数过低      
 *
 * @对应解决方案:
 *     1.增加待绘制的坦克图象
 */

/**
 * @日期: 2018-05-23 (Day_13)
 *
 * @更新内容:
 *     1.增加了菜单界面图形
 *     2.优化了炮弹在GRASS材质下的飞行情况
 *
 * @待解决问题:
 *
 * @对应解决方案:
 *     
 */

/**
 * @日期: 2018-05-24 (Day_14)
 *
 * @更新内容:
 *     1.第一个菜单界面设置完毕
 *     2.设置了第二个菜单的返回内容
 *
 * @待解决问题:
 *     1.BackStep按键双击无反应             //solved
 *
 * @对应解决方案:
 *     1.优化函数                          //to Day_16
 */

/**
 * @日期: 2018-05-26 (Day_16)
 *
 * @更新内容:
 *     1.隐藏了cmd小黑窗
 *     2.修复了Backstep双击问题
 *
 * @待解决问题:
 *
 * @对应解决方案:
 *
 */

/**
 * @日期: 2018-05-28 (Day_18)
 *
 * @更新内容:
 *     1.增加玩家击中敌人判定
 *     2.增加玩家击中友军判定
 *     3.增加了玩家与敌人的实体碰撞
 *     4.增加了玩家射中heart游戏结束判定
 *     5.优化敌人出生效果
 *
 * @待解决问题:
 *     1.玩家在敌人出生点等待会被出生的坦克卡住
 * @对应解决方案:
 *     1.优化游戏机制
 *
 */

/**
 * @日期: 2018-06-06 (Day_26)
 *
 * @更新内容:
 *     1.准备优化炮弹处理机制
 *
 * @待解决问题:
 *     1.当玩家炮弹同时接触不同材质的地图元素时会程序崩溃
 *     2.优化AI
 * @对应解决方案:
 *     1.优化炮弹清除函数
 *
 */