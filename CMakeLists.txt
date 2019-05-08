cmake_minimum_required (VERSION 2.6)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules")

project (tasep-cpp)

find_package(PNG REQUIRED)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(tasep.out src/tasep.cpp)

include_directories(${PNG_INCLUDE_DIRS})
target_link_libraries(tasep.out ${PNG_LIBRARIES})
