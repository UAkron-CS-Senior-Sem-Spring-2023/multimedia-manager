cmake_minimum_required(VERSION 3.23)

project(MultimediaManager
    VERSION 0.1
    DESCRIPTION "A tool to manage multiple messaging accounts simultaneously"
)

set(CMAKE_AUTOMOC ON)

add_executable(MultimediaManager)
target_compile_options(MultimediaManager PRIVATE -Wall)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS NetworkAuth)
target_link_libraries(MultimediaManager
    PRIVATE Qt6::Widgets
    PRIVATE Qt6::NetworkAuth
    PRIVATE curl
)

target_include_directories(MultimediaManager
    PUBLIC src
    PUBLIC extern-headers
)

add_subdirectory(src)