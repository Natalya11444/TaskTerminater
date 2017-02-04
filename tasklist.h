#ifndef TASKLIST_H
#define TASKLIST_H

#include <iostream>
#include <string>
#include<tchar.h>
#include <process.h>
#include <windows.h>
#include <tlhelp32.h>

#include <list>
#include <string>

#include <QDebug>

using namespace std;

class TaskList
{
private:
    class Group {
    public:
        string groupName;
        int quantity;
    };

    Group * taskgroups;
    int groupsMax;
    int groupsQuantity;
    PROCESSENTRY32 pe32;
    HANDLE hProcessSnap;
    HANDLE hProcess;

    void addGroup(string group);
    int getGroupIndex( string group);
    BOOL takeProcessesSnapshot ();
    void enlargeTaskGroups();

public:
    TaskList();
    ~TaskList();
    string* getTaskGroups();
    int groupSize(string groupName);
    int size();
    void termitateProcess(string proc);
    void refreshProcessList();
};

#endif // TASKLIST_H
