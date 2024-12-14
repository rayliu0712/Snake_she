cmake_minimum_required(VERSION 3.10)
project(snake CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SRC_FILES src/*.cpp)

add_executable(snake ${SRC_FILES})
set_target_properties(snake PROPERTIES OUTPUT_NAME "蛇")
target_link_libraries(snake PRIVATE ncursesw fmt)