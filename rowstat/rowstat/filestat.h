/**
 * @File  fiestat.h
 * @Brief 针对指定文件统计其总行数及有效行数
 * @Date  2018-05-27
 */

#ifndef _FILESTAT_
#define _FILESTAT_

#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <iomanip>

#define BLANK 0
#define NOTE  1
#define QUOTE 2
#define VALID 3

const int maxlen = 1000;
class File_Stat;

class File_Stat {
public:
    File_Stat();
    File_Stat(char *path);
    ~File_Stat();

    char *GetFilePath();
    int GetTotalCount();
    int GetValidCount();

    bool OpenFile(char *path);
    void StatWorking();

private:
    std::ifstream file_in;
    char file_path[maxlen / 10];
    char buff[maxlen];
    int total_count;
    int valid_count;
};

#endif // !_FILESTAT_