//#pragma once

#ifndef TASKWINDOW
#define TASKWINDOW

#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QDebug>
#include <QTreeView>
#include <QComboBox>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDesktopWidget>
#include <QString>
#include <QApplication>

#include <iostream>
#include <string>
#include<tchar.h>
#include <process.h>
#include <windows.h>
#include <tlhelp32.h>
#include "windows.h"

#include <list>
#include <string>
#include "tasklist.h"
#include "filemanager.h"

using namespace std;

class TaskWindow:public QObject
{
    Q_OBJECT
private:
    QPushButton * terminateButton;
    QPushButton * refreshButton;
    QGridLayout * layout;
    QComboBox * comboBox;
    QWidget *window;
    QTreeView * tree;
    list<string>* taskList;
    QStandardItem * parentItem;
    TaskList tl;
    QStandardItemModel * model;
    FileManager fileManager;
    void fillComboBox();

public:
    TaskWindow();
    ~TaskWindow();
    void showWindow();
    void terminateProcess();
    void refreshProcessList();
    void saveProcessToDelete();
};
#endif // TASKWINDOW
