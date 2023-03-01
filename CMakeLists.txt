cmake_minimum_required(VERSION 3.23)

project(MultimediaManager
    VERSION 0.1
    DESCRIPTION "A tool to manage multiple messaging accounts simultaneously"
)

add_executable(MultimediaManager)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
target_link_libraries(MultimediaManager
    PRIVATE Qt6::Widgets
    PRIVATE curl
)

add_subdirectory(src)