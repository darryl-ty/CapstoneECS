cmake_minimum_required(VERSION 3.26)
project(ECSExplorations)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
        src/ui/mainwindow.cpp
        src/ui/mainwindow.h
        src/ui/startwindow.cpp
        src/ui/startwindow.h
        src/ui/worldcreationwindow.cpp
        src/ui/worldcreationwindow.h
        src/world/Tile.cpp
        src/world/WorldTile.cpp
)

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
)