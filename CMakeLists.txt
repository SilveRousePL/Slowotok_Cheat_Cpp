cmake_minimum_required(VERSION 3.13)
project(Slowotok_Cheat_Cpp)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Threads)

add_executable(Slowotok_Cheat_Cpp main.cpp UI/MainWindow.h UI/MainWindow.cpp App.cpp App.h Finder.cpp Finder.h File.cpp File.h UI/About.cpp UI/About.h)
target_link_libraries(Slowotok_Cheat_Cpp Qt5::Core)
target_link_libraries(Slowotok_Cheat_Cpp Qt5::Widgets)