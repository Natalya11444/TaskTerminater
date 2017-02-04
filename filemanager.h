#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <QDebug>
#include "windows.h"

using namespace std;

class FileManager
{
private:

    FILE *file;
    const int maxQuantity = 5;
    const char * fileName = "savedProcesses.txt";
    const char * tempFileName = "savedProcessesTemp.txt";
    int quantity;
    string * savedProcesses;

public:
    FileManager();
    ~FileManager();

    bool saveProcess(string process);
    bool addToSavedProcesses(string process);
    string * getSavedProcesses();
    int getQuantity();
    bool isSaved(string process);
    int getMaxQuantity();
};

#endif // FILEMANAGER_H
