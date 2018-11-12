TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt
QT += concurrent

SOURCES += main.cpp \
    useqtthread.cpp

win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501

win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -llibboost_filesystem-mgw49-mt-d-x32-1_68.dll

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -lboost_system-mgw49-mt-d-x32-1_68

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

unix|win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -llibboost_system-mgw49-mt-d-x32-1_68.dll

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -llibboost_thread-mgw49-mt-d-x32-1_68.dll

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -llibboost_serialization-mgw49-mt-d-x32-1_68.dll

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

HEADERS += \
    useqtthread.h


win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -llibboost_context-mgw49-mt-d-x32-1_68.dll

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -llibboost_program_options-mgw49-mt-d-x32-1_68.dll

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68

win32: LIBS += -L$$PWD/../../boost/boost_mingw/lib/ -lboost_chrono-mgw49-mt-d-x32-1_68

INCLUDEPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
DEPENDPATH += $$PWD/../../boost/boost_mingw/include/boost-1_68
