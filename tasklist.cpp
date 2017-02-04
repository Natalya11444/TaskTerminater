#include "tasklist.h"

TaskList::TaskList()
{
    groupsMax = 50;
}

void TaskList::addGroup(string group)
{
    qDebug() << "add group " ;
    // check if it's not the first process with this name
    int ind = getGroupIndex(group);
    // add process if not added yet
    if (ind == -1){
        // check if need to enlarge the size of array with processes
        if (groupsQuantity >= groupsMax)
        {
            enlargeTaskGroups();
        }
        Group gr;
        gr.groupName = group;
        gr.quantity = 1;
        taskgroups[groupsQuantity] = gr;
        groupsQuantity ++;}
    else {
        // increase the numbed of instances if process is added already
        taskgroups[ind].quantity++;
    }
}

// copy existing processes to the bigger array
void TaskList::enlargeTaskGroups()
{
    groupsMax += groupsMax;
    qDebug() << "new groupsMax: " << groupsMax;
    Group * oldGroups = taskgroups;
    taskgroups = new Group [groupsMax];
    for (int i = 0; i < groupsMax/2; i ++)
    {
        taskgroups[i] = oldGroups[i];
    }
}

int TaskList::getGroupIndex( string group)
{
    for (int i = 0; i < groupsQuantity; i ++)
    {
        if(group == taskgroups[i].groupName)
        {
            return i;
        }
    }
    return -1;
}

BOOL TaskList::takeProcessesSnapshot ()
{
    // Take a snapshot of all processes in the system.
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        return(FALSE);
    }

    // Set the size of the structure before using it.
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        // clean the snapshot object
        CloseHandle(hProcessSnap);
        return(FALSE);
    }
    return TRUE;
}

void TaskList::termitateProcess(string proc)
{
    takeProcessesSnapshot();
    qDebug() << "Terminate proc: " << QString::fromStdString(proc);
    bool found = false;
    do
    {
        wchar_t* txt = pe32.szExeFile;
        wstring ws(txt);
        string str(ws.begin(), ws.end());
        string pr = proc;
        if (str == pr)
        {
            found = true;
            DWORD dwDesiredAccess = PROCESS_TERMINATE;
            BOOL  bInheritHandle = FALSE;
            HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, pe32.th32ProcessID);
            BOOL result = TerminateProcess(hProcess, 1);
            if (!result)
            {
                qDebug() << "Failed to close: " << QString::fromStdString(proc);
            }
            else
            {
                qDebug() << "Closed: " << QString::fromStdString(proc);
            }
            CloseHandle(hProcess);
        }
    } while (Process32Next(hProcessSnap, &pe32));
    CloseHandle(hProcessSnap);
    if (!found)
    {
        qDebug() << "Process is not found: " << QString::fromStdString(proc);
    }
}

string * TaskList::getTaskGroups()
{
    qDebug() << "getTaskGroups " ;

    //prevents memory leak when refresh process list
    delete []taskgroups;

    // create array for processes
    taskgroups = new Group [groupsMax];
    groupsQuantity = 0;

    BOOL took = takeProcessesSnapshot();
    if (took)
    {
        qDebug() << "Took Processes Snapshot " ;
    }
    else {
        qDebug() << "Failed to take Processes Snapshot " ;
        return NULL;
    }

    do
    {
        //convert
        wchar_t* txt = pe32.szExeFile;
        wstring ws(txt);
        string str(ws.begin(), ws.end());
        qDebug() << "next process: " << QString::fromStdString(str);
        // save each process
        addGroup(str);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    string* pStr = new string[groupsQuantity];
    for (int i = 0; i < groupsQuantity; i ++)
    {
        pStr[i] = taskgroups[i].groupName;
    }
    return pStr;
}

int TaskList::groupSize(string groupName)
{
    for (int i = 0; i < groupsQuantity; i ++)
    {
        if(taskgroups[i].groupName == groupName)
        {
            return taskgroups[i].quantity;
        }
    }
    return -1;
}

int TaskList::size()
{
    return groupsQuantity;
}

TaskList::~TaskList()
{   
    qDebug()<<"delete object TaskList";
    delete []taskgroups;
    delete &groupsMax;
    delete &groupsQuantity;
    delete &pe32;
    delete hProcessSnap;
    delete hProcess;
}
