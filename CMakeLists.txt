cmake_minimum_required(VERSION 3.6)
project(vee)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -iquote ${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/thread/Core.h src/thread/Core.cpp src/thread/packet/Packets.h src/thread/packet/PDelay.h src/thread/packet/PLazy.h src/ConcurrentQueue.h src/Application.cpp src/Application.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} pthread)