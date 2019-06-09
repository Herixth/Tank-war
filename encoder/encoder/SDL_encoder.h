/**
 * @file  SDL_encoder.h
 * @brief ��ȡԤ�õ�ͼ���Ե�ͼ���н���
 *
 * ����Ԥ������������뵽map/ ���01 - map_total
 * �������:
 *     .I>  ��Surface���������������˳��������Ա��� >���ϵ���_������<
 *     .II> ��Ҫ�õ��ĵ�ͼԪ����4��,�ֱ��ʾΪ:
 *             ->             '0' => air
 *             ->             'a' => wall
 *             ->             'b' => steel
 *             ->             'c' => grass
 *             ->             'd' => water
 *           �ر��,airԪ�ض�Ӧ'0'ֻ��������Ӧ��ʾ,���������airԪ��Ϊ�ղ����б���     
 *     .III> �ö�ʮ�������� >0 - 9 && A - P; ��ĸ��Ϊ��д< �Ե�ͼԪ��λ�ý��м�¼
 *     .IV>  '\'��ʾ�ǿ�һ�еĿ�ʼ,������һ����ʮ��������,������ʾ�б�
 *     .V>   �ڶ�һ�н��б���ʱ,Ԫ�ص��б�����Ԫ������ĺ���,�����Ԫ��������ǰ���ڵ���ͬ,Ԫ�������ʡ��
 *     .VI>  ������������"../../mapcode/level-&&.dat"·���ļ���
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
 * ��ȡĿ��Surface��ָ������λ��x, y(��λ:pixel)���ص�Uint32ֵ
 * @param surface ����ȡ������Ϣ��ƽ��
 * @param x       ����_x
 * @param y       ����_y
 * @return        ����ܻ�ȡ������Ϣ����Uint32��������ֵ��������ܷ���0 
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
 * �������������Ϣ����ȡRGBֵ
 * @param pixel          Uint32���͵�������Ϣ
 * @param mask           ��ȡָ����ɫֵ�Ĺ�����
 * @param shift          ��ȡָ����ɫֵ����λ��
 * @param loss           ��ȡָ����ɫֵ�������
 * @return pixel_element ����ȡ��ɫR/G/B��ֵ
 */
Uint32 Get_PrimaryColor(Uint32 pixel, Uint32 mask, Uint8 shift, Uint8 loss) {
	Uint32 pixel_element = ((pixel & mask) >> shift) << loss;
	//ȡ���������λ
	return pixel_element;
}



/**
 * ���ñ����ַ���
 * @param surface  ָ��SDL_Surfaceָ��
 * @param filename ָ��д�������ļ�
 * @return         ���ز����ͱ�������ʾ�����Ƿ�ɹ�
 */
bool Set_Codestr(SDL_Surface* surface, char filename[], int block_len) {
	std::ofstream encoder(filename, std::ios::out);
	if (!encoder.is_open()) {
		return false;              //�޷����ļ�
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

			if (__IS_AIR_) continue;          //air ����
			if (element == '0') {
				encoder << "/" << (char)(row / block_len < 10 ? row / block_len + '0' : row / block_len - 10 + 'A');
			}

			alpha = 1 * __IS_WALL_ + 2 * __IS_STEEL_ + 3 * __IS_GRASS_ + 4 * __IS_WATER_;        //�ж��Ƿ����element
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
