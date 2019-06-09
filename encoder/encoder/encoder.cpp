#include <algorithm>

#include "SDL_encoder.h"

int main(int argc, char* argv[]) {

	for (int map_num = 12; map_num <= map_total; map_num ++) {
		/* 初始化: 文件路径    图片路径*/
		char filepath[] = "../../mapcode/level-&&.dat";
		char PNGpath[] = "maplist/map_&&.png";

		char *mid = strchr(filepath, '&');
		filepath[mid - filepath]     = map_num / 10 + '0';
		filepath[mid - filepath + 1] = map_num % 10 + '0';
		
		mid = strchr(PNGpath, '&');
		PNGpath[mid - PNGpath]     = map_num / 10 + '0';
		PNGpath[mid - PNGpath + 1] = map_num % 10 + '0';

		SDL_Surface *map_list = IMG_Load(PNGpath);
		Set_Codestr(map_list, filepath, std::min(map_list -> w, map_list -> h) / block_num + 1);
        
		SDL_FreeSurface(map_list);
	}
	Get_Codestr(12);
    system("pause");
	return 0;
}
