
TARGET = ../bnc

CONFIG -= debug
CONFIG += release c++14

QT += printsupport

QMAKE_CFLAGS_ISYSTEM=-I

include(src.pri)

HEADERS +=             app.h \
    Misc.h

SOURCES += bncmain.cpp app.cpp

INCLUDEPATH += /usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.4-svn/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.4-svn/lib/debug/ -lqwt
else:unix: LIBS += -L/usr/local/qwt-6.1.4-svn/lib/ -lqwt

INCLUDEPATH += /usr/local/qwt-6.1.4-svn/include
DEPENDPATH += /usr/local/qwt-6.1.4-svn/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/lib/release/ -lqwtpolar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/lib/debug/ -lqwtpolar
else:unix: LIBS += -L/usr/local/qwtpolar-1.2.1-svn/lib/ -lqwtpolar

INCLUDEPATH += /usr/local/qwtpolar-1.2.1-svn/include
DEPENDPATH += /usr/local/qwtpolar-1.2.1-svn/include
