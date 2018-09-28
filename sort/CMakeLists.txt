cmake_minimum_required(VERSION 3.5)

find_package(PkgConfig REQUIRED)

pkg_check_modules(MASC REQUIRED masc>=1.0.0)

project(sort VERSION 1.0.0)

include(GNUInstallDirs)

set(CMAKE_C_FLAGS "-O0 -g -std=gnu99 -Wall -Werror")

add_executable(${PROJECT_NAME} sort.c)
target_link_libraries(${PROJECT_NAME} ${MASC_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${MASC_INCLUDE_DIRS})
