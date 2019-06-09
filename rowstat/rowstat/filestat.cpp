#include "filestat.h"

File_Stat::File_Stat(char *path) {
    file_in.open(path, std::ios::in);
    memset(buff, 0, sizeof(buff));
    strcpy(file_path, path);
    total_count = 0;
    valid_count = 0;
    if (file_in.fail()) {
        std::cout << path << "open failed" << std::endl;
        exit(1);
    }
}

File_Stat::File_Stat() {
    memset(buff, 0, sizeof(buff));
    total_count = 0;
    valid_count = 0;
}

File_Stat::~File_Stat() {
    file_in.close();
}

char *File_Stat::GetFilePath() {
    return this->file_path;
}

int File_Stat::GetTotalCount() {
    return this->total_count;
}

int File_Stat::GetValidCount() {
    return this->valid_count;
}

bool File_Stat::OpenFile(char *path) {
    strcpy(file_path, path);
    file_in.open(path, std::ios::in);
    return !file_in.fail();
}

void File_Stat::StatWorking() {
    while (!file_in.eof()) {
        file_in.getline(buff, sizeof(buff));
        int length = strlen(buff);
        int row_flag = BLANK;
        for (int inc = 0; inc < length; inc ++) {
            if (buff[inc] != ' ' && buff[inc] != '\t') {
                switch (buff[inc]) {
                    break;
                case '#':
                    if (row_flag != VALID) {
                        row_flag = NOTE;
                    }
                    break;
                case '/': case '*':
                    if (row_flag != VALID) {
                        row_flag = QUOTE;
                    }
                    break;
                default:
                    row_flag = VALID;
                    break;
                }
                if (row_flag == NOTE || row_flag == QUOTE) {
                    break;
                }
            }
        }
        if (row_flag == VALID) {
            valid_count ++;
        }
        total_count ++;
    }
}