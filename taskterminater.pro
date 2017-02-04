QT += widgets

SOURCES       = main.cpp \
    taskwindow.cpp \
    tasklist.cpp \
    filemanager.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/dirview
INSTALLS += target

HEADERS += \
    taskwindow.h \
    tasklist.h \
    filemanager.h
