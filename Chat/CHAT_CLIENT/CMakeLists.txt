cmake_minimum_required(VERSION 3.10.2)
project(r_chat_client)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "-O0 -pedantic-errors -Wextra -Wall -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Wmissing-declarations -Wno-ignored-qualifiers -Wno-unused-parameter")
find_package(Boost 1.62.0 COMPONENTS system chrono filesystem program_options signals date_time thread regex serialization)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()
set(CMAKE_PREFIX_PATH "/opt/soft/qt-sdk/5.12.0/gcc_64")#указать путь к  Qt
set(CMAKE_AUTOMOC ON)#Q_OBJECT - для использования в проекте механизма сигналов и слотов.

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Quick REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
set(SOURCE_FILES
        main.cpp
        recource/QML/qml.qrc
        cpp/ThreadPool.h
        cpp/chatmodel.cpp
        cpp/chatmodel.h
        cpp/client.cpp
        cpp/client.h
        cpp/operatorlayer.cpp
        cpp/operatorlayer.h
        cpp/message.hpp
        cpp/service.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} Qt5::Core Qt5::Quick Qt5::Network)
