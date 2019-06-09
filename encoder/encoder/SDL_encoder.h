/**
 * @file  SDL_encoder.h
 * @brief 读取预置地图，对地图进行解析
 *
 * 按照预定规则编码输入到map/ 编号01 - map_total
 * 编码规则:
 *     .I>  按Surface中像素区域的排列顺序进行线性编码 >从上到下_从左到右<
 *     .II> 需要用到的地图元素有4种,分别表示为:
 *             ->             '0' => air
 *             ->             'a' => wall
 *             ->             'b' => steel
 *             ->             'c' => grass
 *             ->             'd' => water
 *           特别的,air元素对应'0'只是用来对应表示,译码过程视air元素为空不进行编码     
 *     .III> 用二十六进制数 >0 - 9 && A - P; 字母均为大写< 对地图元素位置进行记录
 *     .IV>  '\'表示非空一行的开始,其后紧跟一个二十六进制数,用来表示行标
 *     .V>   在对一行进行编码时,元素的列标排在元素种类的后面,如果的元素种类与前相邻的相同,元素种类可省略
 *     .VI>  编码结果保留在"../../mapcode/level-&&.dat"路径文件下
 *
 * @date  2018/05
 */


#pragma once
#ifndef __SDL_ENCODER_
#define __SDL_ENCODER_
#define __CODE_MAXLEN_ (int)1E5
#define __IS_AIR_   ((int)Red<100&&(int)Green<100&&(int)Blue<100)
#define __IS_WALL_  ((int)Red<200&&(int)Green<100&&(int)Blue<100)
#define __IS_STEEL_ ((int)Red>200&&(int)Green>200&&(int)Blue>200)
#define __IS_GRASS_ ((int)Red<200&&(int)Green>100&&(int)Blue<100)
#define __IS_WATER_ ((int)Red<200&&(int)Green<200&&(int)Blue>200)

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

const int block_num = 26;
const int map_total = 12;

/**
 * 获取目标Surface上指定坐标位置x, y(单位:pixel)像素的Uint32值
 * @param surface 待提取像素信息的平面
 * @param x       坐标_x
 * @param y       坐标_y
 * @return        如果能获取像素信息返回Uint32类型像素值，如果不能返回0 
 */
Uint32 Get_Pixel(SDL_Surface* surface, int x, int y) {
	int Byte_PP = surface -> format -> BytesPerPixel;
	Uint8 *pixel = (Uint8 *)surface -> pixels + y * surface -> pitch + x * Byte_PP;
	switch (Byte_PP) {
	case 1:
		return *pixel;
		break;
	case 2:
		return *(Uint16 *)pixel;
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return pixel[0] << 16 | pixel[1] << 8 | pixel[2];
		else
			return pixel[0] | pixel[1] << 8 | pixel[2] << 16;
		break;
	case 4:
		return *(Uint32 *)pixel;
		break;
	default:
		return 0;
		break;
	}
}



/**
 * 从输入的像素信息中提取RGB值
 * @param pixel          Uint32类型的像素信息
 * @param mask           提取指定颜色值的过滤码
 * @param shift          提取指定颜色值的移位码
 * @param loss           提取指定颜色值的损耗码
 * @return pixel_element 所提取颜色R/G/B的值
 */
Uint32 Get_PrimaryColor(Uint32 pixel, Uint32 mask, Uint8 shift, Uint8 loss) {
	Uint32 pixel_element = ((pixel & mask) >> shift) << loss;
	//取与后两次移位
	return pixel_element;
}



/**
 * 设置编码字符串
 * @param surface  指定SDL_Surface指针
 * @param filename 指定写入编码的文件
 * @return         返回布尔型变量，表示译码是否成功
 */
bool Set_Codestr(SDL_Surface* surface, char filename[], int block_len) {
	std::ofstream encoder(filename, std::ios::out);
	if (!encoder.is_open()) {
		return false;              //无法打开文件
	}
    
	int alpha = 0;
	char element = '0';            //{ '0', 'a', 'b', 'c', 'd' } => { air, wall, steel, grass, water }
	Uint8 Red, Green, Blue;
	SDL_PixelFormat* fmt = surface -> format;
	for (int row = ceil(block_len / 2) + 1; row < surface -> h; row += block_len) {

		for (int col = block_len / 2 + 1; col < surface -> w; col += block_len) {
			Red   = Get_PrimaryColor(Get_Pixel(surface, col, row), fmt -> Rmask, fmt -> Rshift, fmt -> Rloss);
			Green = Get_PrimaryColor(Get_Pixel(surface, col, row), fmt -> Gmask, fmt -> Gshift, fmt -> Gloss);
			Blue  = Get_PrimaryColor(Get_Pixel(surface, col, row), fmt -> Bmask, fmt -> Bshift, fmt -> Bloss);

			if (__IS_AIR_) continue;          //air 跳过
			if (element == '0') {
				encoder << "/" << (char)(row / block_len < 10 ? row / block_len + '0' : row / block_len - 10 + 'A');
			}

			alpha = 1 * __IS_WALL_ + 2 * __IS_STEEL_ + 3 * __IS_GRASS_ + 4 * __IS_WATER_;        //判断是否更新element
			if (alpha - 1 + 'a' != element && alpha) {
				element = (char)(alpha - 1 + 'a');
				encoder << (char)element;
			}

			encoder << (char)(col / block_len < 10 ? col / block_len + '0' : col / block_len - 10 + 'A');
		}
		element = '0';
	}
	encoder.close();
	return true;
}


void Get_Codestr(int map_num) {
	char filepath[] = "../../mapcode/level-&&.dat";
	char *mid = strchr(filepath, '&');
	filepath[mid - filepath]     = map_num / 10 + '0';
	filepath[mid - filepath + 1] = map_num % 10 + '0';

	std::cout << "filepath: " << filepath << std::endl;

	std::ifstream in(filepath);
	char code[__CODE_MAXLEN_] = { 0 };
	while (!in.eof()) {
		in.getline(code, __CODE_MAXLEN_);
		std::cout << code << std::endl;
	}
	in.close();
}

#endif // !__SDL_ENCODER_
