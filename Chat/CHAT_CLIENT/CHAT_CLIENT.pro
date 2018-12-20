QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    client.cpp \
    chatmodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:LIBS += -lsetupapi
win32:LIBS += -lwsock32
win32:LIBS += -lws2_32
win32:DEFINES += _WIN32_WINNT=0x0501



HEADERS += \
    client.h \
    chatmodel.h \
    ../message.h \
    ThreadPool.h
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

win32: LIBS += -L$$PWD/../../../boost_1_69_0/lib/ -llibboost_thread-vc141-mt-gd-x64-1_69

INCLUDEPATH += $$PWD/../../../boost_1_69_0
DEPENDPATH += $$PWD/../../../boost_1_69_0
