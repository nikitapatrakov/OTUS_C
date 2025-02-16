cmake_minimum_required(VERSION 3.28.3)

project(gtk4)

# Исправили опечатку в "STANDARD"
set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GLIB REQUIRED glib-2.0)

# Удалили link_directories, так как это не нужно при использовании target_link_libraries
# include_directories(${GTK4_INCLUDE_DIRS})
# include_directories(${GLIB_INCLUDE_DIRS})

# Исправили опечатку в "GTK4_CFLAGS_OTHER" и переместили add_definitions в target_compile_options
# add_definitions(${GTK4_CFLAGS_OTHER})

add_executable(GTK main.c)

# Используем target_include_directories вместо include_directories
target_include_directories(GTK PRIVATE ${GLIB_INCLUDE_DIRS} ${GTK4_INCLUDE_DIRS})

target_link_libraries(GTK PRIVATE ${GLIB_LIBRARIES} ${GTK4_LIBRARIES})

# Исправили опечатку в "target_complite_options" на "target_compile_options" и переместили флаги сюда
target_compile_options(GTK PRIVATE ${GTK4_CFLAGS_OTHER})