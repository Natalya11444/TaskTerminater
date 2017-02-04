#include "taskwindow.h"

TaskWindow::TaskWindow()
{
    showWindow();
}

void TaskWindow::terminateProcess()
{
    QString selectedProcess = comboBox->currentText();
    tl.termitateProcess(selectedProcess.toStdString());
    // to be sure procces have enough time to be closed
    Sleep(500);
    refreshProcessList();
}

void TaskWindow::refreshProcessList()
{
    parentItem = model->invisibleRootItem();
    // get opened processes
    string* tasks = tl.getTaskGroups();
    // add each process to the list view
    for (int i = 0; i < tl.size(); i ++)
    {
        string nextProc = tasks[i];
        QStandardItem * item = new QStandardItem();
        item->setText(QString::fromStdString(nextProc));
        // get how many instances of the process are opened with the same name
        int groupSize = tl.groupSize(nextProc);
        // if there is not only one instance then the item in the list view will be expandable
        if (groupSize > 1)
        {
            for (int e = 0; e < groupSize; e ++)
            {
                QStandardItem * itemCh = new QStandardItem();
                itemCh->setText(QString::fromStdString(nextProc));
                item ->setChild(e, itemCh);
                qDebug() << "Set child ";
            }
        }
        parentItem->setChild(i, 0, item);
        qDebug() << "Show process: " << QString::fromStdString(nextProc);
    }

    tree->setModel(model);

    QModelIndex index = parentItem->index();
    tree->setRootIndex(index);

    QModelIndex currentIndex = model->index(0,0);
    tree->setCurrentIndex(currentIndex);
}

void TaskWindow::showWindow()
{
    tree = new QTreeView();
    refreshButton = new QPushButton(" Refresh list of processes ");
    terminateButton = new QPushButton(" Terminate selected process ");
    comboBox = new QComboBox();
    comboBox->setInsertPolicy(QComboBox::InsertAtCurrent);

    fillComboBox();

    layout = new QGridLayout;
    layout->addWidget(comboBox, 0, 0);
    layout->addWidget(terminateButton, 0, 1);
    layout->addWidget(refreshButton, 2, 0);

    model = new QStandardItemModel;

    refreshProcessList();

    tree->setAnimated(true);
    tree->setIndentation(20);
    tree->setSortingEnabled(true);
    const QSize availableSize = QApplication::desktop()->availableGeometry(tree).size();
    tree->resize(availableSize / 2);
    tree->setColumnWidth(0, tree->width() / 3);
    layout->addWidget(tree, 1, 0, 1, 2);

    // add on click listeners to the buttons
    QObject::connect(terminateButton,&QPushButton::clicked,this,&TaskWindow::terminateProcess);
    QObject::connect(refreshButton,&QPushButton::clicked,this,&TaskWindow::refreshProcessList);
    QObject::connect(tree, &QTreeView::clicked , this, &TaskWindow::saveProcessToDelete);

    window = new QWidget;
    window->setLayout(layout);
    window->resize(400,500);
    window->show();
}

void TaskWindow::fillComboBox()
{
    int savedPrQuantity = fileManager.getQuantity();
    qDebug() << "savedPrQuantity: " << savedPrQuantity;
    if (savedPrQuantity > 0)
    {
        comboBox->clear();
        string* savedProcesses = fileManager.getSavedProcesses();
        for (int i = 0; i < savedPrQuantity; i ++)
        {
            comboBox->addItem(QString::fromStdString(savedProcesses[i]));
        }
    }
}

void TaskWindow::saveProcessToDelete()
{
    // get selected processes that need to delete
    QModelIndexList list = tree->selectionModel()->selectedIndexes();
    QString proc = list.at(0).data().toString();
    qDebug() << "Selected proc: " << proc;
    if (!fileManager.isSaved(proc.toStdString()))
    {
        qDebug() << "proc is not saved: ";
        fileManager.addToSavedProcesses(proc.toStdString());
        fillComboBox();
        comboBox->setCurrentText(proc);
    }
    comboBox->setCurrentText(proc);
}

TaskWindow::~TaskWindow()
{   
    qDebug()<<"delete object TaskWindow";
    delete terminateButton;
    delete refreshButton;
    delete layout;
    delete tree;
    delete taskList;
    delete window;
    delete parentItem;
    delete model;
}
