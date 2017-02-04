#include "filemanager.h"

FileManager::FileManager()
{
    qDebug()<<"FileManager";
    quantity = 0;
    savedProcesses = new string[maxQuantity];
    getSavedProcesses();
}

bool FileManager::addToSavedProcesses(string process)
{
    qDebug()<<"addToSavedProcesses";
    if (isSaved(process))
    {
        qDebug()<<"process is saved already";
        return true;
    }
    char word[256];
    qDebug()<<"quantity: " << quantity;
    // append new record to the file if quantity < maxQuantity
    if (quantity < maxQuantity)
    {
        qDebug()<<"quantity < maxQuantity";
        errno_t err = fopen_s(&file, fileName, "a");
        if (err)
        {
            qDebug()<<"Couldn't open file for writing ";
            return false;
        }
        strcpy(word, process.c_str());
        fprintf(file, "%s", strcat(word, " "));
        savedProcesses[quantity++] = process;
        fclose(file);
        return true;
    }
    else {
        // delete first recod and add new one if quantity >= maxQuantity
        qDebug()<<"quantity >= maxQuantity";
        errno_t err = fopen_s(&file, tempFileName, "a");
        if (err)
        {
            qDebug()<<"Couldn't open file for writing ";
            return false;
        }
        //rewrite last saved processes from 1 to 5 (+ new one to the end)
        // 0th record will be deleted
        for(int i = 1; i < quantity; i++)
        {
            strcpy(word, savedProcesses[i].c_str());
            fprintf(file, "%s", strcat(word, " "));
        }
        strcpy(word, process.c_str());
        fprintf(file, "%s", strcat(word, " "));

        int rErr = remove(fileName);
        if(rErr)
        {
            qDebug()<<"error in removing file: " << QString::fromStdString(strerror(errno));
        }
        fclose(file);

        int rc = rename(tempFileName, fileName);
        if(rc)
        {
            qDebug()<<"error in renaming file: " << QString::fromStdString(strerror(errno));
            return false;
        }
        return true;
    }
}

string * FileManager::getSavedProcesses()
{
    qDebug()<<"getSavedProcesses1";
    errno_t err = fopen_s(&file, fileName, "r");
    if (err)
    {
        qDebug()<<"File doesn't exist " ;
        return NULL;
    }
    char savedProc [255];
    quantity = 0;
    while (fscanf_s(file, "%s", savedProc, _countof(savedProc) ) != EOF && quantity < maxQuantity)
    {
        qDebug()<<"get next Saved Process";
        savedProcesses[quantity++] =  savedProc;
    }
    fclose(file);
    return savedProcesses;
}

bool FileManager::isSaved(string process)
{
    getSavedProcesses();
    if (quantity == 0)
    {
        qDebug() << "quantity = 0 ";
        return false;
    }
    for (int i = 0; i < quantity; i ++)
    {
        if (process.compare(savedProcesses[i]) == 0)
        {
            qDebug() << "process is saved: " << QString::fromStdString(process);
            return true;
        }
    }
    qDebug() << "process is not saved";
    return false;
}

int FileManager::getQuantity()
{
    qDebug() << "quantity: " << quantity;
    return quantity;
}

int FileManager::getMaxQuantity()
{
    qDebug() << "maxQuantity: " << maxQuantity;
    return maxQuantity;
}


FileManager::~FileManager()
{
    qDebug()<<"delete object FileManager";
    delete file;
    delete [] savedProcesses;
}
