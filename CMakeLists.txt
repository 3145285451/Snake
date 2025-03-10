cmake_minimum_required(VERSION 3.10)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# 只使用 MinGW-w64 64 位编译器
find_program(CMAKE_C_COMPILER NAMES x86_64-w64-mingw32-gcc gcc)
find_program(CMAKE_CXX_COMPILER NAMES x86_64-w64-mingw32-g++ g++)

if(NOT CMAKE_C_COMPILER OR NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "未找到 MinGW-w64 64 位编译器，请检查环境变量！")
endif()

# 检查 MinGW 版本
execute_process(COMMAND ${CMAKE_CXX_COMPILER} --version
                OUTPUT_VARIABLE GCC_VERSION
                OUTPUT_STRIP_TRAILING_WHITESPACE)

message(STATUS "检测到 C++ 编译器: ${CMAKE_CXX_COMPILER}")
message(STATUS "编译器版本: ${GCC_VERSION}")

if(NOT "${GCC_VERSION}" MATCHES "13\\.1")
    message(FATAL_ERROR "需要 MinGW-w64 13.1,但找到的是: ${GCC_VERSION}")
endif()

# 查找 SFML
find_package(SFML 2.6 REQUIRED COMPONENTS graphics window system)

# 添加源代码
add_executable(SnakeGame src/Snake.cpp src/game_utils.cpp src/main.cpp)

# 链接 SFML 库
target_link_libraries(SnakeGame PRIVATE sfml-graphics sfml-window sfml-system)
