# 文件开始

# cmake的最低版本号为3.6
cmake_minimum_required(VERSION 3.6)

# 建立工程
project(SLIC)

# 声明这个工程需要引用OpenCV库
find_package(OpenCV REQUIRED)

# 添加opencv头文件 对应的目录是E:\Toolkits\opencv\opencv\build\include\
# ${变量名}
include_directories(. ${OpenCV_INCLUDE_DIRS})

# 为工程添加源程序
add_executable(SLIC main.cpp slic.h slic.cpp)

#为工程添加静态链接库
target_link_libraries(SLIC ${OpenCV_LIBS})

# 文件结束
