
#include "filestat.h"

const int filenum = 14;

int main(int agrc, char *argv[]) {
    int total = 0;
    int valid = 0;

    char filepath[filenum][maxlen / 10] = {
        "../../main.cpp",
        "../../initialize.h",
        "../../initialize.cpp",
        "../../menu.h",
        "../../menu.cpp",
        "../../operate.h",
        "../../decoder.h",
        "../../encoder/encoder/encoder.cpp",
        "../../encoder/encoder/SDL_encoder.h"
        "../../scoreboard.h",
        "../../scoreboard.cpp", 
        "../../enemyAI.h",
        "../../enemyAI.cpp",
        "../../operate.cpp"
    };

    for (int inc = 0; inc < filenum; inc ++) {
        File_Stat files;
        if (files.OpenFile(filepath[inc])) {
            files.StatWorking();
            std::cout << "filepath:   " << files.GetFilePath() << std::endl;
            std::cout << "total_count: " << files.GetTotalCount() << std::endl;
            std::cout << "valid_count: " << files.GetValidCount() << std::endl << std::endl;
            total += files.GetTotalCount();
            valid += files.GetValidCount();
        }
    }

    /*    获取上一次统计信息   */
    std::string buff;
    std::fstream record;
    record.open("data.dat", std::ios::in);
    int ls_total = 0, ls_valid = 0;
    record >> buff;
    ls_total = atoi(buff.c_str());
    record >> buff;
    ls_valid = atoi(buff.c_str());

    std::cout << "_______________________________________" << std::endl;
    std::cout << "File num:   " << filenum << std::endl;
    std::cout << "In total:   " << total << std::endl;
    std::cout << "In valid:   " << valid << std::endl;
    std::cout << "valid percent: " << std::setprecision(4) << (1.0 * valid) / total * 100 << "%" << std::endl << std::endl;

    std::cout << "Last total: " << ls_total << std::endl;
    std::cout << "Last valid: " << ls_valid << std::endl << std::endl;

    std::cout << "Increment: " << std::endl;
    std::cout << "total: " << total - ls_total << std::endl;
    std::cout << "valid: " << valid - ls_valid << std::endl;
    record.close();

    record.open("data.dat", std::ios::out);
    record << total << std::endl;
    record << valid << std::endl;

    record.close();
    std::cout << "_______________________________________" << std::endl;
#ifdef _DEBUG
    system("pause");
#endif // _DEBUG

    return 0;
}