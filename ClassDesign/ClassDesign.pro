QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CBst.cpp \
    Clist.cpp \
    StoneGame.cpp \
    bstwidget.cpp \
    data.cpp \
    listwidget.cpp \
    main.cpp \
    mypushbutton.cpp \
    searchwidget.cpp \
    stonegamewidget.cpp \
    widget.cpp

HEADERS += \
    CBst.h \
    Clist.h \
    StoneGame.h \
    bstwidget.h \
    data.h \
    listwidget.h \
    mypushbutton.h \
    searchwidget.h \
    stonegamewidget.h \
    widget.h

FORMS += \
    bstwidget.ui \
    listwidget.ui \
    searchwidget.ui \
    stonegamewidget.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/keys.txt \
    model/ChenJun.json

RESOURCES += \
    data.qrc
