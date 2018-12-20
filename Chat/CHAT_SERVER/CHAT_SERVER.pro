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

win32: LIBS += -L$$PWD/../../../boost_1_69_0/lib/ -llibboost_atomic-vc141-mt-gd-x64-1_69

INCLUDEPATH += $$PWD/../../../boost_1_69_0
DEPENDPATH += $$PWD/../../../boost_1_69_0

win32: LIBS += -L$$PWD/../../../boost_1_69_0/lib/ -llibboost_context-vc141-mt-sgd-x64-1_69

INCLUDEPATH += $$PWD/../../../boost_1_69_0
DEPENDPATH += $$PWD/../../../boost_1_69_0

win32: LIBS += -L$$PWD/../../../boost_1_69_0/lib/ -llibboost_program_options-vc141-mt-gd-x64-1_69

INCLUDEPATH += $$PWD/../../../boost_1_69_0
DEPENDPATH += $$PWD/../../../boost_1_69_0

win32: LIBS += -L$$PWD/../../../boost_1_69_0/lib/ -llibboost_system-vc141-mt-gd-x64-1_69

INCLUDEPATH += $$PWD/../../../boost_1_69_0
DEPENDPATH += $$PWD/../../../boost_1_69_0
