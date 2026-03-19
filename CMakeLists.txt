cmake_minimum_required(VERSION 3.10)

project(AttractionManagement)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译器为g++
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# 设置输出目录到项目根目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# Qt配置 - 启用自动MOC、RCC、UIC处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt5的Widgets模块
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集公共源文件（不包括 Main 文件）
file(GLOB COMMON_SOURCES 
    "${PROJECT_SOURCE_DIR}/src/graph.cpp"
    "${PROJECT_SOURCE_DIR}/src/tourism.cpp"
)

# CLI 可执行文件
add_executable(run_cli 
    ${COMMON_SOURCES}
    "${PROJECT_SOURCE_DIR}/src/Main_CLI.cpp"
)

# GUI 可执行文件
add_executable(run_gui 
    ${COMMON_SOURCES}
    "${PROJECT_SOURCE_DIR}/src/Main_GUI.cpp"
)

# 只为 GUI 版本链接 Qt5 的 Widgets 库
target_link_libraries(run_gui Qt5::Widgets)
