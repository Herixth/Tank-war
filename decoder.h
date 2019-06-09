/**
 * @File  decoder.h
 * @Brief 用来对地图编码进行解析
 *        对传入的map_set进行设置 
 * @Date  2018/05
 */


#ifndef _DECODER_
#define _DECODER_
#define _MAX_LEN 1000

#include "initialize.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <cctype>

static void DecodingPro(char code[], Map map_set[block_num][block_num]) {
	int row = 0, kinds = 0;
	for (int stat = 0; stat < (int)strlen(code); stat++) {
		if (code[stat] == '/') {
			stat ++;
			row = isdigit(code[stat]) ? code[stat] - '0' : code[stat] - 'A' + 10;
		}
		else if (code[stat] >= 'a' && code[stat] <= 'z') {
			kinds = code[stat] - 'a' + 1;
		}
		else if (isdigit(code[stat]) || isalpha(code[stat])) {
			int col = isdigit(code[stat]) ? code[stat] - '0' : code[stat] - 'A' + 10;
			map_set[row][col].alive = true;
			map_set[row][col].element_kind = kinds;
		}
	}
	/* 对基地进行预置 */
	map_set[23][11].alive = map_set[23][12].alive = map_set[23][13].alive = map_set[23][14].alive = true;
	map_set[23][11].element_kind = map_set[23][12].element_kind = map_set[23][13].element_kind = map_set[23][14].element_kind = 1;
	map_set[24][11].alive = map_set[24][14].alive = map_set[25][11].alive = map_set[25][14].alive = true;
	map_set[24][11].element_kind = map_set[24][14].element_kind = map_set[25][11].element_kind = map_set[25][14].element_kind = 1;
}

/**
* 对地图编码进行解析
* @param num 地图的编号
* @param map_infor 需要进行配置的对象数组
*/
static void DecodeFromDat(int num, Map map_set[block_num][block_num]) {
	char filename[] = "mapcode/level-&&.dat";
	filename[14] = num / 10 + '0';
	filename[15] = num % 10 + '0';
	char code[_MAX_LEN] = { 0 };
	std::ifstream decoders(filename, std::ios::in);
	while (!decoders.eof()) {
		decoders.getline(code, _MAX_LEN);
		DecodingPro(code, map_set);
		memset(code, 0, _MAX_LEN);
	}
}

#endif // !_DECODER_

