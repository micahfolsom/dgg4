cmake_minimum_required(VERSION 3.10)
project(dgg4)
set(APP_NAME sim)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_FLAGS "-g -O2 -Wall -Wextra -Wpedantic")

# Visualization is required
find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(${APP_NAME} ${SOURCES})
target_link_libraries(${APP_NAME} ${Geant4_LIBRARIES})

add_custom_target(copy-macros ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${PROJECT_SOURCE_DIR}/macros ${PROJECT_BINARY_DIR}/macros
)
