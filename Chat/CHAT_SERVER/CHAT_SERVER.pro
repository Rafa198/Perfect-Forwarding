TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    server.cpp

HEADERS += \
    server.h \
    ../message.h

win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501


INCLUDEPATH += $$PWD/../


win32: LIBS += -L$$PWD/../../../boost/boost_mingw_53_32/lib/ -llibboost_atomic-mgw53-mt-d-x32-1_69.dll

INCLUDEPATH += $$PWD/../../../boost/boost_mingw_53_32/include
DEPENDPATH += $$PWD/../../../boost/boost_mingw_53_32/include

win32: LIBS += -L$$PWD/../../../boost/boost_mingw_53_32/lib/ -llibboost_container-mgw53-mt-d-x32-1_69.dll

INCLUDEPATH += $$PWD/../../../boost/boost_mingw_53_32/include
DEPENDPATH += $$PWD/../../../boost/boost_mingw_53_32/include

win32: LIBS += -L$$PWD/../../../boost/boost_mingw_53_32/lib/ -llibboost_context-mgw53-mt-d-x32-1_69.dll

INCLUDEPATH += $$PWD/../../../boost/boost_mingw_53_32/include
DEPENDPATH += $$PWD/../../../boost/boost_mingw_53_32/include

win32: LIBS += -L$$PWD/../../../boost/boost_mingw_53_32/lib/ -lboost_system-mgw53-mt-d-x32-1_69

INCLUDEPATH += $$PWD/../../../boost/boost_mingw_53_32/include
DEPENDPATH += $$PWD/../../../boost/boost_mingw_53_32/include

win32: LIBS += -L$$PWD/../../../boost/boost_mingw_53_32/lib/ -llibboost_program_options-mgw53-mt-d-x32-1_69.dll

INCLUDEPATH += $$PWD/../../../boost/boost_mingw_53_32/include
DEPENDPATH += $$PWD/../../../boost/boost_mingw_53_32/include
