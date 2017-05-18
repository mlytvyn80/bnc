
TEMPLATE = subdirs

CONFIG += ordered c++14

QT += printsupport

QMAKE_CXXFLAGS += -std=c++14


SUBDIRS = src

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.4-svn/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.4-svn/lib/debug/ -lqwt
else:unix: LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.4-svn/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../usr/local/qwt-6.1.4-svn/include
DEPENDPATH += $$PWD/../../../../usr/local/qwt-6.1.4-svn/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/lib/release/ -lqwtpolar
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/lib/debug/ -lqwtpolar
else:unix: LIBS += -L$$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/lib/ -lqwtpolar

INCLUDEPATH += $$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/include
DEPENDPATH += $$PWD/../../../../usr/local/qwtpolar-1.2.1-svn/include

INCLUDEPATH += /usr/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lnewmat
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lnewmat
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lnewmat

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/release/libnewmat.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/debug/libnewmat.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/release/newmat.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/debug/newmat.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libnewmat.a
