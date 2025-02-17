cmake_minimum_required(VERSION 3.28.3)

project(gtk4)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GLIB REQUIRED glib-2.0)


add_executable(GTK main.c)

target_include_directories(GTK PRIVATE ${GLIB_INCLUDE_DIRS} ${GTK4_INCLUDE_DIRS})

target_link_libraries(GTK PRIVATE ${GLIB_LIBRARIES} ${GTK4_LIBRARIES})

target_compile_options(GTK PRIVATE ${GTK4_CFLAGS_OTHER})