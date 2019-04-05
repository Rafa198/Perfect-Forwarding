cmake_minimum_required(VERSION 3.13)
project(chat_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O0 -pedantic-errors -Wextra -Wall -Winit-self -Wold-style-cast -Woverloaded-virtual -Wuninitialized -Wmissing-declarations -Wno-ignored-qualifiers -Wno-unused-parameter")
find_package(Boost 1.58.0 COMPONENTS system filesystem program_options signals date_time thread regex serialization)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

set(SOURCE_FILES
        main.cpp
        ../message.h
        server.cpp
        server.h)
add_executable(qt_chat_server   ${SOURCE_FILES} main.cpp)
target_link_libraries(qt_chat_server  ${Boost_LIBRARIES}  )